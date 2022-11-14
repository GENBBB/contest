#include <sys/wait.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int
child_process()
{
    pid_t pid = fork();
    if (!pid) {
        printf("3 ");
        exit(0);
    } else if (pid == -1) {
        return 1;
    }
    wait(NULL);
    printf("2 ");
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
    wait(NULL);
    printf("1\n");
    return 0;
}