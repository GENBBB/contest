#include <sys/wait.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

void
child_process(char *str)
{
    execlp(str + 1, str + 1, NULL);
    _exit(1);
}

int
main(int argc, char *argv[])
{
    int result = 0, n_proc = 0;
    for (int i = 1; i < argc; i++) {
        if (argv[i][0] == 'p') {
            n_proc++;
        } else if (argv[i][0] == 's') {
            while (n_proc--) {
                int status;
                if (wait(&status) == -1) {
                    return 1;
                }
                if (WIFEXITED(status) && WEXITSTATUS(status) == 0) {
                    result++;
                }
            }
            n_proc = 0;
        } else {
            continue;
        }
        pid_t pid = fork();
        if (!pid) {
            child_process(argv[i]);
        } else if (pid == -1) {
            return 1;
        }
        if (argv[i][0] == 's') {
            int status;
            if (wait(&status) == -1) {
                return 1;
            }
            if (WIFEXITED(status) && WEXITSTATUS(status) == 0) {
                result++;
            }
        }
    }
    while (n_proc--) {
        int status;
        if (wait(&status) == -1) {
            return 1;
        }
        if (WIFEXITED(status) && WEXITSTATUS(status) == 0) {
            result++;
        }
    }
    printf("%d\n", result);
    return 0;
}