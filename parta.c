// parta.c
#define _POSIX_C_SOURCE 200809L
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <errno.h>
#include <string.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "USAGE: parta FILEIN\n");
        return 1;
    }

    int fd = open(argv[1], O_RDONLY);
    if (fd < 0) {
        fprintf(stderr, "ERROR: cannot open '%s': %s\n", argv[1], strerror(errno));
        return 1;
    }

    unsigned long cnt_upper = 0, cnt_lower = 0, cnt_digit = 0, cnt_space = 0, cnt_other = 0;
    unsigned char buf[8];

    for (;;) {
        ssize_t n = read(fd, buf, sizeof buf);
        if (n < 0) {
            if (errno == EINTR) continue;
            fprintf(stderr, "ERROR: read: %s\n", strerror(errno));
            close(fd);
            return 1;
        }
        if (n == 0) break; // EOF

        for (ssize_t i = 0; i < n; ++i) {
            int c = buf[i];
            if (isupper(c)) cnt_upper++;
            else if (islower(c)) cnt_lower++;
            else if (isdigit(c)) cnt_digit++;
            else if (isspace(c)) cnt_space++;
            else cnt_other++;
        }
    }

    close(fd);

    printf("Upper,%lu\n",  cnt_upper);
    printf("Lower,%lu\n",  cnt_lower);
    printf("Number,%lu\n", cnt_digit);
    printf("Space,%lu\n",  cnt_space);
    printf("Others,%lu\n", cnt_other);

    return 0;
}
