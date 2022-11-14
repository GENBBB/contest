#include <sys/wait.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int
child_process()
{
    return 0;
}

int
main(int argc, char *argv[])
{
    pid_t pid = fork();
    if (!pid) {
        exit(child_process());
    } else if (pid == -1) {
        return 1;
    }
}