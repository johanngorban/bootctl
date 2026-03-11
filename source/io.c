#include "io.h"
#include <stdint.h>
#include <stdio.h>
#include <errno.h>
#include <poll.h>
#include <unistd.h> 

#define BCP_MAX_RETRIES  5
#define BCP_TIMEOUT_MS   1000

int read_n(int fd, uint8_t *buf, size_t n) {
    size_t received = 0;
    int    attempts = 0;

    while (received < n) {
        if (attempts >= BCP_MAX_RETRIES) {
            fprintf(stderr, "Too many attempts to get any response. Try later\n");
            return -1;
        }

        struct pollfd pfd = {
            .fd = fd,
            .events = POLLIN
        };

        int rc = poll(&pfd, 1, BCP_TIMEOUT_MS);

        if (rc < 0) {
            if (errno == EINTR) continue;
            perror("read: poll");
            return -1;
        }
        if (rc == 0) {
            attempts++;
            continue;
        }

        ssize_t r = read(fd, buf + received, n - received);
        if (r < 0) {
            if (errno == EINTR) continue;
            perror("read");
            return -1;
        }
        if (r == 0) {
            fprintf(stderr, "read: порт закрыт\n");
            return -1;
        }

        received += (size_t)r;
    }

    return 0;
}

int write_n(int fd, const uint8_t *buf, size_t n) {
    size_t written  = 0;
    int attempts = 0;

    while (written < n) {
        if (attempts >= BCP_MAX_RETRIES) {
            fprintf(stderr, "Too many attempts (%d)\n", BCP_MAX_RETRIES);
            return -1;
        }

        struct pollfd pfd = {
            .fd = fd,
            .events = POLLOUT 
        };
        int rc = poll(&pfd, 1, BCP_TIMEOUT_MS);

        if (rc < 0) {
            if (errno == EINTR) {
                continue;
            }

            perror("bcp: poll");
            return -1;
        }

        if (rc == 0) {
            attempts++;
            continue;
        }

        ssize_t got = write(fd, buf + written, n - written);
        if (got < 0) {
            perror("bcp: write");
            return -1;
        }

        written += (size_t) got;
    }

    return 0;
}