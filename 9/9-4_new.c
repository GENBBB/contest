#include <sys/wait.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

void
child_process(int N)
{
    for (int i = 2; i < N + 1; i++) {
        pid_t pid = fork();
        if (pid) {
            printf(" %d", i);
            fflush(stdout);
            if (i != N) {
                wait(NULL);
            }
            exit(0);
        }
    }
    exit(0);
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
    pid_t pid = fork();
    if (!pid) {
        child_process(N);
    }
    wait(NULL);
    putchar('\n');
    return 0;
}
