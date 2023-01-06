#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>

int
main(int argc, char *argv[])
{
    int fd_prev = STDIN_FILENO;
    pid_t *pids = calloc(argc - 1, sizeof(*pids));
    if (pids == NULL) {
        return 1;
    }
    for (int i = 1; i < argc; i++) {
        int fd[2];
        if (i != argc - 1) {
            if (pipe2(fd, O_CLOEXEC) == -1) {
                for (int j = 0; j < i - 1; j++) {
                    kill(pids[j], SIGKILL);
                }
                for (int j = 0; j < i - 1; j++) {
                    wait(NULL);
                }
                free(pids);
                return 1;
            }
        }
        pids[i - 1] = fork();
        if (!pids[i - 1]) {
            if (dup2(fd_prev, STDIN_FILENO) == -1) {
                _exit(1);
            }
            if (i != argc -1) {
                if (dup2(fd[1], STDOUT_FILENO) == -1) {
                    _exit(1);
                }
            }
            execlp(argv[i], argv[i], NULL);
            _exit(1);
        } else if (pids[i - 1] == -1) {
            for (int j = 0; j < i - 1; j++) {
                kill(pids[j], SIGKILL);
            }
            for (int j = 0; j < i - 1; j++) {
                wait(NULL);
            }
            free(pids);
            return 1;
        }
        if (i != argc - 1) {
            if (close(fd[1]) == -1) {
                for (int j = 0; j < i; j++) {
                    kill(pids[j], SIGKILL);
                    wait(NULL);
                }
                free(pids);
                return 1;
            }
        }
        if (i != 1) {
            if (close(fd_prev) == -1) {
                for (int j = 0; j < i; j++) {
                    kill(pids[j], SIGKILL);
                    wait(NULL);
                }
                free(pids);
                return 1;
            }
        }
        fd_prev = fd[0];
    }
    for (int i = 0; i < argc - 1; i++) {
        wait(NULL);
    }
    free(pids);
    return 0;
}