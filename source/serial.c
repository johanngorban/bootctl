#include "serial.h"
#include <termios.h>
#include <errno.h>
#include <stdio.h>

int serial_port_init(int fd, int speed, int parity, bool blocking) {
    struct termios tty;
    if (tcgetattr(fd, &tty) != 0) {
        printf("Error %d from tcgetattr\n", errno);
        return -1;
    }

    cfsetospeed(&tty, speed);
    cfsetispeed(&tty, speed);

    tty.c_cflag = (tty.c_cflag & ~CSIZE) | CS8;
    tty.c_iflag &= ~IGNBRK; // Disable break processing
    tty.c_lflag = 0;        // No signaling char, no echo
    tty.c_oflag = 0;
    tty.c_cc[VMIN] = (int) blocking;     // Read doesn't block
    tty.c_cc[VTIME] = 10;    // 1 seconds read timeout

    tty.c_iflag &= ~(IXON | IXOFF | IXANY);
    tty.c_cflag |= (CLOCAL | CREAD);
    tty.c_cflag &= ~(PARENB | PARODD);

    tty.c_cflag |= parity;
    tty.c_cflag &= ~CSTOPB;
    tty.c_cflag &= ~CRTSCTS;

    if (tcsetattr(fd, TCSANOW, &tty) != 0) {
        printf("Error %d from tcsetattr\n", errno);
        return -1;
    }

    return 0;
}