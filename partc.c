// partc.c
#define _POSIX_C_SOURCE 200809L
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "USAGE: partc FILEIN\n");
        return 1;
    }
    const char *file_in = argv[1];

    int pfd[2];
    if (pipe(pfd) < 0) {
        fprintf(stderr, "ERROR: pipe: %s\n", strerror(errno));
        return 1;
    }

    pid_t pid = fork();
    if (pid < 0) {
        fprintf(stderr, "ERROR: fork: %s\n", strerror(errno));
        close(pfd[0]); close(pfd[1]);
        return 1;
    }

    if (pid > 0) {
        // Parent: become parta; write to pipe
        close(pfd[0]); // close read end
        if (dup2(pfd[1], STDOUT_FILENO) < 0) {
            fprintf(stderr, "ERROR: dup2 (parent): %s\n", strerror(errno));
            close(pfd[1]);
            return 1;
        }
        close(pfd[1]);
        char *const args[] = {"./parta", (char*)file_in, NULL};
        execv("./parta", args);
        fprintf(stderr, "ERROR: execv ./parta failed: %s\n", strerror(errno));
        _exit(127);
    } else {
        // Child: become sort; read from pipe
        close(pfd[1]); // close write end
        if (dup2(pfd[0], STDIN_FILENO) < 0) {
            fprintf(stderr, "ERROR: dup2 (child): %s\n", strerror(errno));
            close(pfd[0]);
            _exit(1);
        }
        close(pfd[0]);
        char *const args[] = {"sort", "-t,", "-k2", "-n", NULL};
        execvp("sort", args); // search PATH for sort
        fprintf(stderr, "ERROR: execvp sort failed: %s\n", strerror(errno));
        _exit(127);
    }
}

