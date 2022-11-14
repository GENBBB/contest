#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

int
main (int argc, char *argv[])
{
    int fd = open(argv[1], O_RDWR);
    if (fd == -1) {
        return 1;
    }
    struct stat file_stat;
    if (stat(argv[1], &file_stat) == -1) {
        return 1;
    }
    int n;
    if (sscanf(argv[2], "%d", &n) != 1) {
        return 1;
    }
    n = ((file_stat.st_size / sizeof(double)) < n) ? (file_stat.st_size / sizeof(double)) : n;
    double prev, curr;
    if (read(fd, &prev, sizeof(prev)) != sizeof(prev)) {
        return 1;
    }
    for (int i = 1; i < n; i++){
        if (read(fd, &curr, sizeof(curr)) != sizeof(curr)) {
            return 1;
        }
        curr -= prev;
        if (lseek(fd, i * sizeof(curr), SEEK_SET) == -1) {
            return 1;
        }
        if (write(fd, &curr, sizeof(curr)) != sizeof(curr)) {
            return 1;
        }
        if (lseek(fd, (i + 1) * sizeof(curr), SEEK_SET) == -1) {
            return 1;
        }
        prev = curr;
    }
    return 0;
}
