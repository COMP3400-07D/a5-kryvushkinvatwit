// partb.c
#define _POSIX_C_SOURCE 200809L
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "USAGE: partb FILEIN FILEOUT\n");
        return 1;
    }
    const char *file_in  = argv[1];
    const char *file_out = argv[2];

    int fd = open(file_out, O_WRONLY | O_CREAT | O_TRUNC, 0770);
    if (fd < 0) {
        fprintf(stderr, "ERROR: cannot open '%s': %s\n", file_out, strerror(errno));
        return 1;
    }

    if (dup2(fd, STDOUT_FILENO) < 0) {
        fprintf(stderr, "ERROR: dup2: %s\n", strerror(errno));
        close(fd);
        return 1;
    }
    close(fd); // no longer needed

    // Print header then flush before exec
    printf("Category,Count\n");
    fflush(stdout);

    char *const args[] = {"./parta", (char*)file_in, NULL};
    execv("./parta", args);

    // If we get here, exec failed
    fprintf(stderr, "ERROR: execv ./parta failed: %s\n", strerror(errno));
    return 1;
}
