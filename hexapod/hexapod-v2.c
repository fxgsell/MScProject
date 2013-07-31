/*
 *	hexapod-v2.c -- control program for the AVR mega on the hexapod robot
 *	Copyright (C) 2013 Fred Barnes, University of Kent <frmb@kent.ac.uk>
 *
 *	This program is free software; you can redistribute it and/or modify
 *	it under the terms of the GNU General Public License as published by
 *	the Free Software Foundation; either version 2 of the License, or
 *	(at your option) any later version.
 *
 *	This program is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *	GNU General Public License for more details.
 *
 *	You should have received a copy of the GNU General Public License
 *	along with this program; if not, write to the Free Software
 *	Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301 USA
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>

#define cbi(reg,bit) (_SFR_BYTE(reg) &= ~_BV(bit))
#define sbi(reg,bit) (_SFR_BYTE(reg) |= _BV(bit))

/*{{{  connectivity / notes*/
/*
 *	Note: the previous version of the board needed to multiplex analogue inputs;  this one doesn't.
 *	Additionally, this version talks through to the SSC32 board on TX1/RX1 (PD2/3).
 */

/*
 *	Onboard LED is on PB7.
 *	ADC0-5 are foot-sensor inputs (typical 0-3.5v); AREF on 5v feed.
 *	ADC8-11 are tilt-sensor inputs.
 *	OC0B(PG5) is PWM output for range-sensor-turntable.
 *	PE3-5 are range-sensor output/input.
 *	TX0/RX0(PE1-0) are the host interface.
 *	TX1/RX1(PD3-2) are the SSC32 board interface (at 38400).
 */

/*
 *	The host uses a simple request-response serial protocol to communicate with
 *	the AVR/Arduino; strings ASCII formatted terminating with a carriage return (0x0d / 13).
 *
 *	"R\x0d"  (read sensors) -> "R leg0 leg1 leg2 leg3 leg4 leg5 leg6 til0 til1 til2 til3\x0d"  (values formatted as 16-bit hex)
 *	"Mn\x0d"  (set ping-scan mode to n, where n is '0' .. '1') -> "Mn\x0d"
 *	"S...\x0d"  (send string to SSC-32, no response expected)
 *	"s...\x0d"  (send string to SSC-32, response expected) -> { "s...\x0d"  (response from SSC-32) || "TIMEOUT\x0d" }
 *	"Pn\x0d"  (read ping map n=[0..2]) -> "P dst0 dst1 dst2 ... dst19\x0d"
 *	"In\x0d"  (set digital IO mode to n, where n is '0' .. '3') -> "In\x0d"
 *	"Dxxyyzz\x0d"  (set digital outputs to 22-29=xx, 37-30=yy, 49-42=zz)
 *	"Tnnnn\x0d"  (set digital output timeout to nnnn hex)
 */

/*}}}*/


/*{{{  global vars*/

#define BUFFER_SIZE 128

#define ANALOG_VAL_COUNT	(10)
static volatile uint16_t analog_values[ANALOG_VAL_COUNT];		/* values from the 10 analog sensors (6 feet, 4 tilt) */
static volatile int analog_sel;						/* which one we're currently reading (10 = finished) */

static volatile unsigned char host_rxbuf[BUFFER_SIZE];			/* incoming bytes from host */
static volatile int host_rxcount;					/* receive count */
static volatile unsigned char host_rxerr;				/* non-zero if receive error */
static volatile unsigned char host_txbuf[BUFFER_SIZE];			/* outgoing bytes to host */
static volatile int host_txcount;					/* transmit count */
static volatile int host_txgone;					/* bytes gone out */

static volatile unsigned char ssc_rxbuf[BUFFER_SIZE];			/* incoming bytes from SSC-32 */
static volatile int ssc_rxcount;					/* receive count */
static volatile unsigned char ssc_rxerr;				/* non-zero if receive error */
static volatile unsigned char ssc_txbuf[BUFFER_SIZE];			/* outgoing bytes to SSC-32 */
static volatile int ssc_txcount;					/* transmit count */
static volatile int ssc_txgone;						/* bytes gone out */

static volatile unsigned char ssc_wait;					/* if waiting for SSC response */

static volatile unsigned char timer0_pwm;				/* PWM period for turntable (14 -> 23 -> 33) */
#define TTABLE_MAP_LEN	(20)
static volatile int ttable_idx;						/* particular table index */
static volatile uint16_t ttable_dmap[TTABLE_MAP_LEN][3];		/* data map for each servo position */
static unsigned char ttable_pmap[TTABLE_MAP_LEN] = {23, 25, 27, 29, 31, 33, 32, 30, 28, 26, 24, 22, 20, 18, 16, 14, 15, 17, 19, 21};

#define TMODE_NONE	0x00
#define TMODE_CYCLE	0x01

static volatile unsigned char ttable_mode;				/* TMODE_... */

static volatile uint16_t ping_dist;					/* up-counter for distance */
static volatile unsigned char ping_left;				/* bit-field */
static volatile unsigned char ping_ready;				/* do we have ping data? */

#define DIOMODE_NONE	0x00						/* plain/static outputs */
#define DIOMODE_CYCLEC	0x02						/* cycle bits clockwise */
#define DIOMODE_CYCLECC	0x03						/* cycle bits anticlockwise */

static volatile unsigned char dio_mode;
static volatile unsigned char dio_vals[3];				/* PORTA, PORTC and PORTL values */
static volatile uint16_t dio_timeout;					/* timeout for digital I/O output (100ms default) */

#define P_HOST_TX	(0x01)
#define P_HOST_RX	(0x02)
#define P_SSC_TX	(0x04)
#define P_SSC_RX	(0x08)
#define P_ADC		(0x10)
#define P_TIMER		(0x20)
#define P_PING		(0x40)

static volatile unsigned char pflags;					/* program state flags */

static volatile unsigned char timer0_step;				/* which time-slice we're in, controls sensor reading/etc. */

/*}}}*/


/*{{{  static void global_init (void)*/
/*
 *	initialises the assortment of global variables
 */
static void global_init (void)
{
	int i;

	for (i=0; i<ANALOG_VAL_COUNT; i++) {
		analog_values[i] = 0x0000;
	}
	analog_sel = 0;

	host_rxcount = 0;
	host_rxerr = 0;
	host_txcount = 0;
	host_txgone = 0;

	ssc_rxcount = 0;
	ssc_rxerr = 0;
	ssc_txcount = 0;
	ssc_txgone = 0;
	ssc_wait = 0;

	timer0_pwm = 23;

	ttable_idx = 0;
	for (i=0; i<TTABLE_MAP_LEN; i++) {
		ttable_dmap[i][0] = 0x0000;
		ttable_dmap[i][1] = 0x0000;
		ttable_dmap[i][2] = 0x0000;
	}
	ttable_mode = TMODE_CYCLE;

	ping_dist = 0;
	ping_left = 0x00;
	ping_ready = 0;

	dio_mode = DIOMODE_NONE;
	dio_vals[0] = 0x00;
	dio_vals[1] = 0x00;
	dio_vals[2] = 0x00;
	dio_timeout = 0x0000;						/* interpreted as "disabled" */

	pflags = 0;
	timer0_step = 0;

	return;
}
/*}}}*/
/*{{{  static void microsleep (uint32_t us)*/
/*
 *	fixed micro-delay
 */
static void microsleep (uint32_t us)
{
	for (; us; us--) {
#if F_CPU == 16000000
		__asm__ __volatile__ ("		\n" \
			"	nop		\n" \
			"	nop		\n" \
			"	nop		\n" \
			"	nop		\n" \
			"	nop		\n" \
			"	nop		\n" \
			"	nop		\n" \
			"	nop		\n" \
			"	nop		\n" \
			"	nop		\n" \
			"	nop		\n" \
			"	nop		\n" \
		);
#else
#error unsupported F_CPU value in microsleep()
#endif
	}
}
/*}}}*/
#if 0
/*{{{  static int lnstrcmp (unsigned char *s1, int l1, unsigned char *s2)*/
/*
 *	local string compare, length of string 1 given
 */
static int lnstrcmp (unsigned char *s1, int l1, unsigned char *s2)
{
	int i;
	unsigned char *p1 = s1;
	unsigned char *p2 = s2;

	for (i=0; (*p1 == *p2) && (i<l1) && (*p2 != '\0'); i++);
	if ((i == l1) && (*p2 == '\0')) {
		/* match */
		return 0;
	}
	return 1;
}
/*}}}*/
#endif
/*{{{  static int ltstrcpy (unsigned char *dst, const char *src)*/
/*
 *	local source-null-terminated string copy, returns count
 */
static int ltstrcpy (unsigned char *dst, const char *src)
{
	int i;
	unsigned char *p1 = dst;
	unsigned char *p2 = (unsigned char *)src;

	for (i=0; *p2 != '\0'; p1++, p2++, i++) {
		*p1 = *p2;
	}
	return i;
}
/*}}}*/
/*{{{  static unsigned char hexnibble (const unsigned char val)*/
/*
 *	returns hexadecimal digit for a nibble
 */
static unsigned char hexnibble (const unsigned char val)
{
	if (val < 10) {
		return '0' + val;
	} else if (val < 16) {
		return 'a' + (val - 10);
	}
	return '?';
}
/*}}}*/
/*{{{  static unsigned char unhexnibble (const unsigned char val)*/
/*
 *	returns numeric nibble for a hexadecimal character
 */
static unsigned char unhexnibble (const unsigned char val)
{
	if ((val >= '0') && (val <= '9')) {
		return (val - '0');
	} else if ((val >= 'a') && (val <= 'f')) {
		return (val - 'a') + 10;
	} else if ((val >= 'A') && (val <= 'F')) {
		return (val - 'A') + 10;
	}

	return 0xff;
}
/*}}}*/


/*{{{  ISR (USART0_UDRE_vect)*/
/*
 *	USART0 data-register-empty interrupt
 */
ISR (USART0_UDRE_vect)
{
	if (host_txgone == host_txcount) {
		/* nothing left to transmit, send newline, enable TXCIE0, disable UDRIE0 */
		_SFR_BYTE(UCSR0B) |= _BV(TXCIE0);
		_SFR_BYTE(UCSR0B) &= ~(_BV(UDRIE0));
		_SFR_BYTE(UDR0) = '\x0d';
	} else {
		_SFR_BYTE(UDR0) = host_txbuf[host_txgone];
		host_txgone++;
	}
}
/*}}}*/
/*{{{  ISR (USART0_TX_vect)*/
/*
 *	USART0 transmitted character gone interrupt (after shift-out and empty UDR0)
 */
ISR (USART0_TX_vect)
{
	pflags |= P_HOST_TX;
	/* disable this interrupt */
	_SFR_BYTE(UCSR0B) &= ~(_BV(TXCIE0));
}
/*}}}*/
/*{{{  ISR (USART0_RX_vect)*/
/*
 *	USART0 character received interrupt
 */
ISR (USART0_RX_vect)
{
	unsigned char ch, status;

	status = _SFR_BYTE (UCSR0A);
	ch = _SFR_BYTE (UDR0);

	if (status & (_BV(FE0) | _BV(DOR0) | _BV(UPE0))) {
		/* error condition, await newline */
		host_rxerr = 1;
	} else if (host_rxerr) {
		/* previously in error-condition, newline? */
		if (ch == '\x0d') {
			host_rxcount = 0;
			host_rxerr = 0;
		}
		/* else throw away (good character in error condition) */
	} else if (pflags & P_HOST_RX) {
		/* means we haven't finished processing the last one yet, so mark error and throw away the rest */
		host_rxerr = 1;
	} else {
		/* sensible character received */
		if ((ch == '\x0d') || (host_rxcount == BUFFER_SIZE)) {
			/* full or end, report as read complete */
			pflags |= P_HOST_RX;
		} else {
			host_rxbuf[host_rxcount] = ch;
			host_rxcount++;
		}
	}
}
/*}}}*/
/*{{{  ISR (USART1_UDRE_vect)*/
/*
 *	USART1 data-register-empty interrupt
 */
ISR (USART1_UDRE_vect)
{
	if (ssc_txgone == ssc_txcount) {
		/* nothing left to transmit, send newline, enable TXCIE1, disable UDRIE1 */
		_SFR_BYTE(UCSR1B) |= _BV(TXCIE1);
		_SFR_BYTE(UCSR1B) &= ~(_BV(UDRIE1));
		_SFR_BYTE(UDR1) = '\x0d';
	} else {
		_SFR_BYTE(UDR1) = ssc_txbuf[ssc_txgone];
		ssc_txgone++;
	}
}
/*}}}*/
/*{{{  ISR (USART1_TX_vect)*/
/*
 *	USART1 transmitted character gone interrupt (after shift-out and empty UDR0)
 */
ISR (USART1_TX_vect)
{
	pflags |= P_HOST_TX;
	/* disable this interrupt */
	_SFR_BYTE(UCSR1B) &= ~(_BV(TXCIE1));
}
/*}}}*/
/*{{{  ISR (USART1_RX_vect)*/
/*
 *	USART1 character received interrupt
 */
ISR (USART1_RX_vect)
{
	unsigned char ch, status;

	status = _SFR_BYTE (UCSR1A);
	ch = _SFR_BYTE (UDR1);

	if (status & (_BV(FE1) | _BV(DOR1) | _BV(UPE1))) {
		/* error condition, await newline */
		ssc_rxerr = 1;
	} else if (ssc_rxerr) {
		/* previously in error-condition, newline? */
		if (ch == '\x0d') {
			ssc_rxcount = 0;
			ssc_rxerr = 0;
		}
		/* else throw away (good character in error condition) */
	} else if (pflags & P_SSC_RX) {
		/* means we haven't finished processing the last one yet, so mark error and throw away the rest */
		ssc_rxerr = 1;
	} else {
		/* sensible character received */
		if ((ch == '\x0d') || (ssc_rxcount == BUFFER_SIZE)) {
			/* full or end, report as read complete */
			pflags |= P_SSC_RX;
		} else {
			ssc_rxbuf[ssc_rxcount] = ch;
			ssc_rxcount++;
		}
	}
}
/*}}}*/
/*{{{  ISR (ADC_vect)*/
/*
 *	ADC conversion complete interrupt
 */
ISR (ADC_vect)
{
	pflags |= P_ADC;
	analog_values[analog_sel] = 0x0000;
	analog_values[analog_sel] = _SFR_BYTE(ADCL);
	analog_values[analog_sel] = ((uint16_t)(_SFR_BYTE(ADCH)) << 8) & 0xff00;
}
/*}}}*/
/*{{{  ISR (TIMER0_OVF_vect)*/
/*
 *	timer0 overflow interrupt (16ms)
 */
ISR (TIMER0_OVF_vect)
{
	pflags |= P_TIMER;
}
/*}}}*/
/*{{{  ISR (TIMER1_OVF_vect)*/
/*
 *	interrupt for timer1 (~30us timer)
 */
ISR (TIMER1_OVF_vect)
{
	unsigned char tmp = (_SFR_BYTE (PINE) & 0x38) >> 3;

	/* reset for next timeout */
	_SFR_BYTE(TCNT1H) = 255;
	_SFR_BYTE(TCNT1L) = (256 - 60);

	ping_dist++;
	if (ping_left == 0x08) {
		/* means we're waiting for signals to go high */
		if (tmp && (ping_dist > 26)) {
			ping_left = 0x07;
		}
	} else {
		/* waiting for one or more to go low */
		tmp ^= ping_left;
		if (tmp & 0x04) {
			ttable_dmap[ttable_idx][2] = ping_dist;
			ping_left &= 0x03;
		}
		if (tmp & 0x02) {
			ttable_dmap[ttable_idx][1] = ping_dist;
			ping_left &= 0x05;
		}
		if (tmp & 0x01) {
			ttable_dmap[ttable_idx][0] = ping_dist;
			ping_left &= 0x06;
		}
	}
	if (!ping_left) {
		pflags |= P_PING;
	}
		
}
/*}}}*/
/*{{{  ISR (TIMER3_OVF_vect)*/
/*
 *	interrupt for timer3 (programmable timer)
 */
ISR (TIMER3_OVF_vect)
{
	/* reset for next timeout */
	_SFR_BYTE(TCNT3H) = (unsigned char)((dio_timeout >> 8) & 0xff);
	_SFR_BYTE(TCNT3L) = (unsigned char)(dio_timeout & 0xff);

	if (dio_mode == DIOMODE_CYCLEC) {
		/* rotate bits clockwise [right] and change on ports */
		unsigned char tmp = dio_vals[2] & 0x01;

		dio_vals[2] = ((dio_vals[2] >> 1) & 0x7f) | ((dio_vals[1] << 7) & 0x80);
		dio_vals[1] = ((dio_vals[1] >> 1) & 0x7f) | ((dio_vals[0] << 7) & 0x80);
		dio_vals[0] = ((dio_vals[0] >> 1) & 0x7f) | ((tmp << 7) & 0x80);

		_SFR_BYTE(PORTA) = dio_vals[0];
		_SFR_BYTE(PORTC) = dio_vals[1];
		_SFR_BYTE(PORTL) = dio_vals[2];
	} else if (dio_mode == DIOMODE_CYCLECC) {
		/* rotate bits anticlockwise [left] and change on ports */
		unsigned char tmp = dio_vals[0] & 0x80;

		dio_vals[0] = ((dio_vals[0] << 1) & 0xfe) | ((dio_vals[1] >> 7) & 0x01);
		dio_vals[1] = ((dio_vals[1] << 1) & 0xfe) | ((dio_vals[2] >> 7) & 0x01);
		dio_vals[2] = ((dio_vals[2] << 1) & 0xfe) | ((tmp >> 7) & 0x01);

		_SFR_BYTE(PORTA) = dio_vals[0];
		_SFR_BYTE(PORTC) = dio_vals[1];
		_SFR_BYTE(PORTL) = dio_vals[2];
	}
}
/*}}}*/


/*{{{  static void serial_init (void)*/
/*
 *	initialises the two serial interfaces:
 *	USART0: 38400, 8N1
 *	USART1: 38400, 8N1
 */
static void serial_init (void)
{
	/* USART0: 38400 */
	_SFR_BYTE (UBRR0H) = (((F_CPU / 38400) / 16) - 1) >> 8;
	_SFR_BYTE (UBRR0L) = (((F_CPU / 38400) / 16) - 1);

	/* USART0: RX,TX enable, 8N1, RX-complete */
	_SFR_BYTE (UCSR0C) = _BV(UCSZ01) | _BV(UCSZ00);
	_SFR_BYTE (UCSR0B) = _BV(RXEN0) | _BV(TXEN0) | _BV(RXCIE0);

	/* USART1: 38400 */
	_SFR_BYTE (UBRR0H) = (((F_CPU / 38400) / 16) - 1) >> 8;
	_SFR_BYTE (UBRR1L) = (((F_CPU / 38400) / 16) - 1);

	/* USART1: RX,TX enable, 8N1, RX-complete */
	_SFR_BYTE (UCSR1C) = _BV(UCSZ11) | _BV(UCSZ10);
	_SFR_BYTE (UCSR1B) = _BV(RXEN1) | _BV(TXEN1) | _BV(RXCIE1);
}
/*}}}*/
/*{{{  static void serial_host_starttx (void)*/
/*
 *	enable the to-host serial transmitter, assumes the buffer is full and ready to go
 */
static void serial_host_starttx (void)
{
	host_txgone = 1;
	_SFR_BYTE(UDR0) = host_txbuf[0];
	_SFR_BYTE (UCSR0B) |= _BV(UDRIE0);		/* interrupt may trigger immediately */
}
/*}}}*/
/*{{{  static void serial_ssc_starttx (void)*/
/*
 *	enable the to-SSC-32 serial transmitter, assumes the buffer is full and ready to go
 */
static void serial_ssc_starttx (void)
{
	ssc_txgone = 1;
	_SFR_BYTE(UDR1) = ssc_txbuf[0];
	_SFR_BYTE (UCSR1B) |= _BV(UDRIE1);		/* interrupt may trigger immediately */
}
/*}}}*/
/*{{{  static void timer0_init (void)*/
/*
 *	initialises timer/counter0, used for PWM output for head turntable.
 */
static void timer0_init (void)
{
	_SFR_BYTE(DDRG) |= _BV(DDG5);		/* output on PG5/OC0B */

	/* fast PWM mode, clear OC0B on compare-match, set OC0B at BOTTOM (non-inverting mode) */
	_SFR_BYTE(OCR0A) = 0xfa;		/* TOP value (whole period = 16ms) */
	_SFR_BYTE(OCR0B) = timer0_pwm;		/* 23 = approx 1.47ms */
	_SFR_BYTE(TCCR0A) = _BV(COM0B1) | _BV(WGM00) | _BV(WGM01);
	_SFR_BYTE(TCCR0B) = _BV(WGM02) | _BV(CS02) | _BV(CS00);		/* fast PWM, divisor /1024 */
	_SFR_BYTE(TIMSK0) = _BV(TOIE0);		/* interrupt when we hit TOP (OCR0A) */
	/* timer0 will be running now */
}
/*}}}*/
/*{{{  static void timer0_setpwm (void)*/
/*
 *	resets the PWM timer for timer0, used for head turntable
 */
static void timer0_setpwm (void)
{
	_SFR_BYTE(OCR0B) = timer0_pwm;
}
/*}}}*/
/*{{{  static void timer3_init (void)*/
/*
 *	initialises timer/counter3, used for DIO outputs
 */
static void timer3_init (void)
{
	_SFR_BYTE(TCCR3A) = 0x00;
	_SFR_BYTE(TCCR3B) = 0x00;
}
/*}}}*/
/*{{{  static void timer3_start (void)*/
/*
 *	starts timer3, prescale /256  (16us ticks at 16 MHz)
 */
static void timer3_start (void)
{
	_SFR_BYTE(TCCR3B) = 0x00;		/* stop */
	_SFR_BYTE(TCNT3H) = (unsigned char)((dio_timeout >> 8) & 0xff);
	_SFR_BYTE(TCNT3L) = (unsigned char)(dio_timeout & 0xff);
	_SFR_BYTE(TCCR3B) = _BV(CS32);
}
/*}}}*/
/*{{{  static void timer3_stop (void)*/
/*
 *	stops timer3
 */
static void timer3_stop (void)
{
	_SFR_BYTE(TCCR3B) = 0x00;		/* stop */
}
/*}}}*/
/*{{{  static void adc_start_convert (void)*/
/*
 *	start ADC conversion for the specified channel (0-9) in analog_sel
 *	prescale set to /128
 */
static void adc_start_convert (void)
{
	_SFR_BYTE(ADCSRA) &= ~(_BV(ADEN));		/* toggle ADC off/on */

	if (analog_sel < 6) {
		/* A0 - A5 */
		_SFR_BYTE(ADMUX) = (analog_sel & 0x1f);	/* low-order 5 bits */
		_SFR_BYTE(ADCSRB) = 0x00;
	} else {
		/* A8 - A11 */
		_SFR_BYTE(ADMUX) = ((analog_sel - 6) & 0x1f);
		_SFR_BYTE(ADCSRB) = _BV(MUX5);
	}
	_SFR_BYTE(ADCSRA) = _BV(ADEN) | _BV(ADIE) | _BV(ADPS2) | _BV(ADPS1) | _BV(ADPS0);	/* enable, interrupt-enable, prescale /128 */

	_SFR_BYTE(ADCSRA) |= _BV(ADSC);			/* start conversion */
}
/*}}}*/
/*{{{  static void adc_init (void)*/
/*
 *	initialises the ADC
 */
static void adc_init (void)
{
	analog_sel = 0;
	_SFR_BYTE(ADMUX) = 0x00;
	_SFR_BYTE(ADCSRA) = 0x00;		/* all off */
	_SFR_BYTE(ADCSRB) = 0x00;

	_SFR_BYTE(DIDR0) = 0x3f;		/* disable digital inputs on A0-A5 */
	_SFR_BYTE(DIDR1) = 0x0f;		/* and A8-A11 */
}
/*}}}*/
/*{{{  static void adc_disable (void)*/
/*
 *	disables the ADC
 */
static void adc_disable (void)
{
	_SFR_BYTE(ADMUX) = 0x00;
	_SFR_BYTE(ADCSRA) = 0x00;		/* all off */
	_SFR_BYTE(ADCSRB) = 0x00;
}
/*}}}*/
/*{{{  static void ping_init (void)*/
/*
 *	initialise the PING))) interface
 */
static void ping_init (void)
{
	_SFR_BYTE(DDRE) |= 0x38;		/* PE3,PE4,PE5 output */
	_SFR_BYTE(PORTE) &= ~0x38;		/* PE3-5 low */

	_SFR_BYTE(TCCR1A) = 0x00;
	_SFR_BYTE(TCCR1B) = 0x00;		/* timer1 basic and stopped */
}
/*}}}*/
/*{{{  static void ping_scan_start (void)*/
/*
 *	starts the ping sensor scan
 */
static void ping_scan_start (void)
{
	/* drive start outputs high for 5us */
	_SFR_BYTE(PORTE) |= 0x38;
	microsleep (5);
	_SFR_BYTE(PORTE) &= ~0x38;

	/* set ports to input, tristate */
	_SFR_BYTE(DDRE) &= ~0x38;
	_SFR_BYTE(PORTE) &= ~0x38;

	ttable_dmap[ttable_idx][0] = 0x0000;
	ttable_dmap[ttable_idx][1] = 0x0000;
	ttable_dmap[ttable_idx][2] = 0x0000;

	ping_dist = 0;
	ping_left = 0x08;

	/* setup TIMER1 to trigger once every 29us, or thereabouts */
	_SFR_BYTE(TIMSK1) = _BV (TOIE1);
	_SFR_BYTE(TCNT1H) = 0xff;
	_SFR_BYTE(TCNT1L) = (256 - 60);
	_SFR_BYTE(TCCR1B) = _BV(CS11);		/* go, prescale /8 */
}
/*}}}*/
/*{{{  static void ping_scan_end (void)*/
/*
 *	tidies up after a ping))) scan has completed
 */
static void ping_scan_end (void)
{
	/* stop TIMER1 */
	_SFR_BYTE (TCCR1B) = 0x00;

	/* set PE3-5 output low */
	_SFR_BYTE(DDRE) |= 0x38;
	_SFR_BYTE(PORTE) &= ~0x38;

	ping_left = 0;
}
/*}}}*/
/*{{{  static void aux_init (void)*/
/*
 *	initialises aux. interface (on the digital I/O pins)
 */
static void aux_init (void)
{
	_SFR_BYTE (DDRA) = 0xff;		/* port A (22-29) all output */
	_SFR_BYTE (DDRC) = 0xff;		/* port C (37-30) all output */
	_SFR_BYTE (DDRL) = 0xff;		/* port L (49-42) all output */

	_SFR_BYTE (PORTA) = 0x00;		/* port A all low */
	_SFR_BYTE (PORTC) = 0x00;		/* port C all low */
	_SFR_BYTE (PORTL) = 0x00;		/* port L all low */
}
/*}}}*/



/*{{{  int main (void)*/
/*
 *	start here (after bootloader has done its thing)
 */
int main (void)
{
	cli ();				/* disable interrupts whilst we setup */

	global_init ();
	serial_init ();
	timer0_init ();
	adc_init ();
	ping_init ();
	aux_init ();
	timer3_init ();

	set_sleep_mode (SLEEP_MODE_IDLE);

	for (;;) {
		cli ();
		if (pflags == 0x00) {
			sleep_enable ();
			sei ();
			sleep_cpu ();
			sleep_disable ();
		}
		sei ();

		if (pflags & P_HOST_TX) {
			/*{{{  serial transmit to host complete*/
			cli ();
			pflags &= ~P_HOST_TX;
			sei ();

			/*}}}*/
		}
		if (pflags & P_SSC_TX) {
			/*{{{  serial transmit to SSC-32 complete*/
			cli ();
			pflags &= ~P_SSC_TX;
			sei ();

			/*}}}*/
		}
		if (pflags & P_HOST_RX) {
			/*{{{  serial receive from host complete*/
			if ((host_rxcount == 1) && (host_rxbuf[0] == 'R')) {
				/*{{{  read sensors*/
				int i, j = 0;

				host_txbuf[j++] = 'R';
				for (i=0; i<ANALOG_VAL_COUNT; i++) {
					host_txbuf[j++] = ' ';
					host_txbuf[j++] = hexnibble ((analog_values[i] >> 12) & 0x0f);
					host_txbuf[j++] = hexnibble ((analog_values[i] >> 8) & 0x0f);
					host_txbuf[j++] = hexnibble ((analog_values[i] >> 4) & 0x0f);
					host_txbuf[j++] = hexnibble (analog_values[i] & 0x0f);
				}
				host_txcount = j;

				serial_host_starttx ();
				/*}}}*/
			} else if ((host_rxcount == 2) && (host_rxbuf[0] == 'M')) {
				/*{{{  set mode*/
				switch (host_rxbuf[1]) {
				case '0':
					ttable_mode = TMODE_NONE;
					break;
				case '1':
					ttable_mode = TMODE_CYCLE;
					break;
				default:
					break;
				}
				host_txbuf[0] = 'M';
				host_txbuf[1] = hexnibble (ttable_mode & 0x0f);
				host_txcount = 2;

				serial_host_starttx ();
				/*}}}*/
			} else if ((host_rxcount > 1) && (host_rxbuf[0] == 'S')) {
				/*{{{  send string to SSC-32, no response*/
				int i;

				for (i=0; i<(host_rxcount - 1); i++) {
					ssc_txbuf[i] = host_rxbuf[i+1];
				}
				ssc_txcount = host_rxcount - 1;

				serial_ssc_starttx ();
				/*}}}*/
			} else if ((host_rxcount > 1) && (host_rxbuf[0] == 's')) {
				/*{{{  send string to SSC-32, expect response*/
				int i;

				for (i=0; i<(host_rxcount - 1); i++) {
					ssc_txbuf[i] = host_rxbuf[i+1];
				}
				ssc_txcount = host_rxcount - 1;

				serial_ssc_starttx ();
				ssc_wait = 1;
				/*}}}*/
			} else if ((host_rxcount == 2) && (host_rxbuf[0] == 'P')) {
				/*{{{  read ping distance map 0..2*/
				int sel, i, j = 0;

				switch (host_rxbuf[1]) {
				case '0':
				default:
					sel = 0;
					break;
				case '1':
					sel = 1;
					break;
				case '2':
					sel = 2;
					break;
				}

				host_txbuf[j++] = 'P';
				for (i=0; i<TTABLE_MAP_LEN; i++) {
					uint16_t tmp = ttable_dmap[i][sel];

					host_txbuf[j++] = ' ';
					host_txbuf[j++] = hexnibble ((tmp >> 12) & 0x0f);
					host_txbuf[j++] = hexnibble ((tmp >> 8) & 0x0f);
					host_txbuf[j++] = hexnibble ((tmp >> 4) & 0x0f);
					host_txbuf[j++] = hexnibble (tmp & 0x0f);
				}
				host_txcount = j;

				serial_host_starttx ();
				/*}}}*/
			} else if ((host_rxcount == 2) && (host_rxbuf[0] == 'I')) {
				/*{{{  set digital IO mode*/
				switch (host_rxbuf[1]) {
				case '0':
				default:
					dio_mode = DIOMODE_NONE;
					break;
				case '2':
					dio_mode = DIOMODE_CYCLEC;
					break;
				case '3':
					dio_mode = DIOMODE_CYCLECC;
					break;
				}

				host_txbuf[0] = 'I';
				host_txbuf[1] = hexnibble (dio_mode & 0x0f);

				host_txcount = 2;

				serial_host_starttx ();

				if (dio_mode == DIOMODE_NONE) {
					timer3_stop ();
				} else {
					timer3_start ();
				}
				/*}}}*/
			} else if ((host_rxcount == 7) && (host_rxbuf[0] == 'D')) {
				/*{{{  set digital IO values*/
				dio_vals[0] = (unhexnibble (host_rxbuf[1]) << 4) | unhexnibble (host_rxbuf[2]);
				dio_vals[1] = (unhexnibble (host_rxbuf[3]) << 4) | unhexnibble (host_rxbuf[4]);
				dio_vals[2] = (unhexnibble (host_rxbuf[5]) << 4) | unhexnibble (host_rxbuf[6]);

				/*}}}*/
			} else if ((host_rxcount == 5) && (host_rxbuf[0] == 'T')) {
				/*{{{  set digital IO timeout*/
				uint16_t val = ((uint16_t)(unhexnibble (host_rxbuf[1])) << 12) | ((uint16_t)(unhexnibble (host_rxbuf[2])) << 8) | \
						((uint16_t)(unhexnibble (host_rxbuf[3])) << 4) | ((uint16_t)(unhexnibble (host_rxbuf[4])));

				dio_timeout = (65536 - val);
				/*}}}*/
			}

			host_rxcount = 0;

			cli ();
			pflags &= ~P_HOST_RX;
			sei ();
			/*}}}*/
		}
		if (pflags & P_SSC_RX) {
			/*{{{  serial receive from SSC-32 complete*/
			if (ssc_wait) {
				/* we were waiting for this, return to host */
				int i;

				host_txbuf[0] = 's';
				for (i=0; i<ssc_rxcount; i++) {
					host_txbuf[i+1] = ssc_rxbuf[i];
				}
				host_txcount = ssc_rxcount + 1;

				serial_host_starttx ();
				ssc_wait = 0;
			}

			cli ();
			pflags &= ~P_SSC_RX;
			sei ();
			/*}}}*/
		}
		if (pflags & P_ADC) {
			/*{{{  ADC conversion complete*/

			/* start the next one */
			analog_sel++;
			if (analog_sel == 10) {
				/* stop */
				adc_disable ();
			} else {
				adc_start_convert ();
			}

			cli ();
			pflags &= ~P_ADC;
			sei ();
			/*}}}*/
		}
		if (pflags & P_PING) {
			/*{{{  ping scan complete*/
			ping_ready = 1;
			ping_scan_end ();

			cli ();
			pflags &= ~P_PING;
			sei ();
			/*}}}*/
		}
		if (pflags & P_TIMER) {
			/*{{{  16ms timer*/
			if (ssc_wait > 0) {
				/* means we were waiting for the SSC-32 to respond, check for timeout */
				ssc_wait++;
				if (ssc_wait > 30) {
					/* means about 500ms have passed, give up */
					host_txcount = ltstrcpy ((unsigned char *)host_txbuf, "TIMEOUT");
					serial_host_starttx ();
					ssc_wait = 0;
				}
			}

			switch (timer0_step) {
			case 0x00:
				/* set servo position */
				timer0_pwm = ttable_pmap[ttable_idx];
				timer0_setpwm ();
				timer0_step++;
				break;
			case 0x01:
			case 0x02:
			case 0x03:
				timer0_step++;
				/* spend ~48ms idle */
				break;
			case 0x04:
				/* start analog input reading */
				analog_sel = 0;
				adc_start_convert ();
				timer0_step++;
				break;
			case 0x05:
				/* waiting for ADC conversions to finish */
				if (analog_sel == 10) {
					/* yep, done */
					timer0_step++;
				}
				/* else keep waiting.. */
				break;
			case 0x06:
			case 0x07:
				/* spend a bit more time idle */
				timer0_step++;
				break;
			case 0x08:
				ping_scan_start ();
				timer0_step++;
				break;
			case 0x09:
				if (ping_ready) {
					timer0_step++;
					ping_ready = 0;
				}
				/* else keep waiting.. */
				break;
			case 0x0a:
				/* next position */
				if (ttable_mode == TMODE_CYCLE) {
					ttable_idx++;
					if (ttable_idx == TTABLE_MAP_LEN) {
						ttable_idx = 0;
					}
				}
				timer0_step++;
				break;
			default:
				/* all done, loop */
				timer0_step = 0;
				break;
			}

			cli ();
			pflags &= ~P_TIMER;
			sei ();
			/*}}}*/
		}
	}

	return 0;
}
/*}}}*/


