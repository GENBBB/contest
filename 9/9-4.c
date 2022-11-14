#include <sys/wait.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int
child_process(int n, int N)
{
    if (n == N + 1) {
        return 0;
    }
    printf(" %d", n);
    fflush(stdout);
    pid_t pid = fork();
    if (!pid) {
        exit(child_process(n + 1, N));
    } else if (pid == -1) {
        return 1;
    }
    wait(NULL);
    return 0;
}

int
main(int argc, char *argv[])
{
    int N;
    if (scanf("%d", &N) != 1) {
        return 1;
    }
    printf("1");
    fflush(stdout);
    int n = 1;
    pid_t pid = fork();
    if (!pid) {
        exit(child_process(n + 1, N));
    } else if (pid == -1) {
        return 1;
    }
    wait(NULL);
    putchar('\n');
    return 0;
}