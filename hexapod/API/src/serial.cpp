#include <string>
#include <iostream>

#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <termios.h>
#include <unistd.h>

#include "serial.hpp"

Serial::Serial(const char *portname)
{
  struct termios tty;
  int speed = B38400;
  int parity = 0;
  
  fd = open(portname, O_RDWR | O_SYNC);
  if (fd < 0) {
    fprintf(stderr, "error %d opening %s: %s", errno, portname, strerror (errno));
    return; // THROW
  }

  memset(&saved_state, 0, sizeof tty);
  if (tcgetattr (fd, &saved_state) != 0)
  {
    fprintf(stderr, "error %d from tcgetattr", errno);
    return;
    // THROW return -1;
  }
  memcpy ((char *)&tty, (char *)&saved_state, sizeof (struct termios));

  cfsetospeed(&tty, speed);
  cfsetispeed(&tty, speed);

  tty.c_iflag &= ~(IGNBRK|BRKINT|PARMRK|ISTRIP|INLCR|IGNCR|ICRNL|IXON);
  tty.c_oflag &= ~OPOST;
  tty.c_lflag &= ~(ECHO|ECHONL|ICANON|ISIG|IEXTEN);
  tty.c_cflag &= ~(CSIZE|PARENB|CSTOPB);
	tty.c_cflag |= CS8;

  if (tcsetattr (fd, TCSAFLUSH, &tty) != 0)
  {
    fprintf(stderr, "error %d from tcsetattr", errno);
    return;
    // THROWreturn -1;
  }

  write("M0\x0d"); // trunoff this stupid radar
  write("M0\x0d"); // trunoff this stupid radar
  std::cout << "Serial initialized" << std::endl;
}

Serial::~Serial() {
  if (tcsetattr (fd, TCSAFLUSH, &saved_state) < 0) {
    fprintf (stderr, "Error: failed to restore serial-port terminal state: %s\n", strerror (errno));
  }
  close(fd);
}

/*{{{  int decode_hex_byte (char b1, char b2, unsigned char *tptr)*/
/*
 *	turns 2 hex characters into an unsigned byte.  returns 0 on success, -1 on error
 */
int decode_hex_byte (char b1, char b2, unsigned char *tptr)
{
	*tptr = 0;
	if ((b1 >= '0') && (b1 <= '9')) {
		*tptr = ((b1 - '0') << 4);
	} else if ((b1 >= 'a') && (b1 <= 'f')) {
		*tptr = (((b1 - 'a') + 10) << 4);
	} else if ((b1 >= 'A') && (b1 <= 'F')) {
		*tptr = (((b1 - 'A') + 10) << 4);
	} else {
		return -1;
	}
	if ((b2 >= '0') && (b2 <= '9')) {
		*tptr |= ((b2 - '0') & 0x0f);
	} else if ((b2 >= 'a') && (b2 <= 'f')) {
		*tptr |= (((b2 - 'a') + 10) & 0x0f);
	} else if ((b2 >= 'A') && (b2 <= 'F')) {
		*tptr |= (((b2 - 'A') + 10) & 0x0f);
	} else {
		return -1;
	}
	return 0;
}

static unsigned char *string_deescape (const unsigned char *str, int *len)
{
	unsigned char *newstr = (unsigned char *)malloc (strlen ((const char *)str) + 1);
	unsigned char *ch, *dh;

	if (len) {
		*len = 0;
	}
	for (ch = (unsigned char *)str, dh = newstr; *ch != '\0'; ch++, dh++) {
		if (*ch == '\\') {
			/* escape sequence */
			ch++;
			switch (*ch) {
			case '\\': *dh = '\\'; break;
			case 'n': *dh = '\n'; break;
			case 'r': *dh = '\r'; break;
			case 't': *dh = '\t'; break;
			case 'a': *dh = '\a'; break;
			case 'b': *dh = '\b'; break;
			case 'f': *dh = '\f'; break;
			case 'v': *dh = '\v'; break;
			case 'x':
				/* hex byte */
				if (decode_hex_byte (ch[1], ch[2], (unsigned char *)dh)) {
					fprintf (stderr, "Error: bad hex escape at char %d\n", (int)(ch - str));
					free (newstr);
					return NULL;
				}
				ch += 2;
				break;
			default:
				fprintf (stderr, "Error: unsupported escape character \'%c\' at char %d\n", *ch, (int)(ch - str));
				free (newstr);
				return NULL;
			}
		} else {
			*dh = *ch;
		}
	}
	if (len) {
		*len = (int)(dh - newstr);
	}
	*dh = '\0';

	return newstr;
}

int Serial::write(const std::string &s) {
  int gone = 0;
  int tries = 5;
  struct termios lterm;
  int len = 0;
  unsigned char * msg = string_deescape((const unsigned char *)s.c_str(), &len);

  /* do the actual write */
  while (gone < len) {
  	int n = ::write (fd, msg + gone, len - gone);
  
  	if ((n < 0) && (errno == EAGAIN)) {
  		if (!tries) {
        return n; //THROE -1
  		}
  		microdelay (10000);
  		tries--;
  		continue;		/* while() */
  	} else if (n < 0) {
  		fprintf (stderr, "Error: write error to serial port: %s\n", strerror (errno));
      return n; //THROW -1
  	}
  	tries = 5;
  	gone += n;
  }

  /* need to wait for the data to actually go out -- setting attributes should cause this.. */
  if (tcgetattr (fd, &lterm) == 0) 
     tcsetattr (fd, TCSAFLUSH, &lterm);
  free(msg);
}












