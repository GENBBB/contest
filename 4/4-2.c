#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

int
main(int argc, char *argv[])
{
    enum {
        SECOND_ARG = 2
    };
    int fd = open(argv[1], O_RDWR);
    if (fd == -1) {
        return 1;
    }
    off_t n;
    if (sscanf(argv[SECOND_ARG], "%lld", &n) != 1) {
        return 1;
    }
    double prev, curr;
    if (read(fd, &prev, sizeof(prev)) != sizeof(prev)) {
        return 0;
    }
    for (int i = 1; i < n; i++) {
        if (read(fd, &curr, sizeof(curr)) != sizeof(curr)) {
            return 0;
        }
        curr -= prev;
        if (lseek(fd, i * sizeof(curr), SEEK_SET) == -1) {
            return 1;
        }
        if (write(fd, &curr, sizeof(curr)) != sizeof(curr)) {
            return 1;
        }
        prev = curr;
    }
    if (close(fd) == -1) {
        return 1;
    }
    return 0;
}
