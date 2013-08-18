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

	printf("Serial initialized\n");
}

Serial::~Serial() {
	if (tcsetattr (fd, TCSAFLUSH, &saved_state) < 0) {
		fprintf (stderr, "Error: failed to restore serial-port terminal state: %s\n", strerror (errno));
	}
	close(fd);
}

