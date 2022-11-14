#include <sys/wait.h>
#include <unistd.h>

int
process(char *str)
{
    pid_t pid = fork();
    if (!pid) {
        execlp(str, str, NULL);
        _exit(1);
    } else if (pid == -1) {
        return 1;
    }
    int status;
    wait(&status);
    if (WIFEXITED(status)) {
        if (WEXITSTATUS(status) == 0) {
            return 0;
        }
    }
    return 1;
}

int
main(int argc, char *argv[])
{
    if (process(argv[1])) {
        if (process(argv[2])) {
            return 1;
        } else {
            return process(argv[3]);
        }
    } else {
        return process(argv[3]);
    }
}