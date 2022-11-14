#include <stdio.h>
#include <limits.h>
#include <dirent.h>
#include <unistd.h>
#include <fcntl.h>

extern ssize_t getcwd2(int fd, char *buf, size_t size);

int
main(int argc, char *argv[]) {
    int fd = open(argv[1], O_RDONLY);
    char buf[100];
    printf("%d\n", getcwd2(fd, buf, 100));
    printf("%s\n", buf);
    return buf[0];
}