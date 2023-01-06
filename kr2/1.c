//
// Created by gennadii on 12/18/22.
//
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

void
child_process(int out, int max, int i)
{
    pid_t pid = fork();
    if (!pid) {
        if (i < max) {
            proc(num * 2 + 1, maxdep, idep + 1);
        }
        printf("%d\n", out);
        fflush(stdout);
        if (i < max) {
            proc(num * 2 + 2, maxdep, idep + 1);
        }
        _exit(0);
    } else if (pid > 0) {
        wait( NULL);
    }
}

int
main(int argc, char *argv[])
{
    int max;
    sscanf(argv[1], "%d", &max);
    child_process()
    return 0;
}