#include <sys/wait.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

void
child_process(char *argv[])
{
    if(close(0) == -1) {
        _exit(42);
    }
    if(close(1) == -1) {
        _exit(42);
    }
    if(close(2) == -1) {
        _exit(42);
    }
    int fd1 = open(argv[2], O_RDONLY);
    if (fd1 == -1) {
        _exit(42);
    }
    int fd2 = open(argv[3], O_CREAT | O_APPEND | O_WRONLY, 0660);
    if (fd2 == -1) {
        _exit(42);
    }
    int fd3 = open(argv[4], O_CREAT | O_TRUNC | O_WRONLY, 0660);
    if (fd3 == -1) {
        _exit(42);
    }
    execlp(argv[1], argv[1], NULL);
    _exit(42);
}

int
main(int argc, char *argv[])
{
    pid_t pid = fork();
    if (!pid) {
        child_process(argv);
    } else if (pid == -1) {
        return 1;
    }
    int status;
    wait(&status);
    printf("%d\n", status);
    return 0;
}