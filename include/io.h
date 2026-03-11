#pragma once

#include <stdlib.h>
#include <stdint.h>

int read_n(int fd, uint8_t *buf, size_t n);

int write_n(int fd, const uint8_t *buf, size_t n);