#include <sys/wait.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>

enum
{
    STR_SIZE = 8
};

int
child_process(int num)
{
    char str[STR_SIZE];
    int value;
    int fd = dup(0);
    lseek(fd, num * STR_SIZE, SEEK_SET);
    if (read(fd, str, STR_SIZE) != STR_SIZE) {
        return 1;
    }
    close(fd);
    sscanf(str, "%d", &value);
    printf("%d %d\n", num+1, value * value);
    return 0;
}

int
main(int argc, char *argv[])
{
    for (int i = 0; i < 3; i++) {
        pid_t pid = fork();
        if (!pid) {
            exit(child_process(i));
        } else if (pid == -1) {
            return 1;
        }
    }
    for (int i = 0; i < 3; i++) {
        wait(NULL);
    }
    return 0;
}