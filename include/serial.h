#pragma once

#include <stdbool.h>
#include <termios.h>

int serial_port_init(int fd, int speed, int parity, bool blocking);