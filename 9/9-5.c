#include <sys/wait.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int
child_process(void)
{
    int value;
    if (scanf("%d", &value) != 1) {
        return 0;
    }
    pid_t pid = fork();
    if (!pid) {
        exit(child_process());
    } else if (pid == -1) {
        return 1;
    }
    int stat;
    wait(&stat);
    if (WEXITSTATUS(stat) == 1) {
        return 1;
    }
    printf("%d\n", value);
    return 0;
}

int
main(int argc, char *argv[]) {
    pid_t pid = fork();
    if (!pid) {
        exit(child_process());
    } else if (pid == -1) {
        return 1;
    }
    int stat;
    wait(&stat);
    if (WEXITSTATUS(stat) == 1) {
        printf("-1\n");
    }
    return 0;
}