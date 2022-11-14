#include <sys/wait.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int
child_process(char *str, int num)
{
    int value;
    sscanf(str, "%d", &value);
    printf("%d %d\n", num+1, value * value);
    return 0;
}

int
main(int argc, char *argv[])
{
    char *strings[3];
    char s1[8], s2[8], s3[8];
    strings[0] = s1;
    strings[1] = s2;
    strings[2] = s3;
    for (int i = 0; i < 3; i++) {
        if (read(0, strings[i], 8) != 8) {
            return 1;
        }
    }
    for (int i = 0; i < 3; i++) {
        pid_t pid = fork();
        if (!pid) {
            exit(child_process(strings[i], i));
        } else if (pid == -1) {
            return 1;
        }
    }
    for (int i = 0; i < 3; i++) {
        wait(NULL);
    }
    return 0;
}