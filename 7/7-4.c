#include <unistd.h>
#include <sys/stat.h>
#include <stdio.h>
#include <inttypes.h>
#include <fcntl.h>

int
main (int argc, char *argv[])
{
    int fd = open(argv[1], O_RDWR);
    if (fd == -1) {
        return -1;
    }
    struct stat fstat;
    if (stat(argv[1], &fstat) != 0) {
        return -1;
    }
    int32_t var = 0;
    while (scanf("%" SCNd32, &var) == 1) {
        if (var > 0) {
            var--;
            char buf = 0;
            if (fstat.st_size > var / 8) {
                if (lseek(fd, var / 8, SEEK_SET) == -1) {
                    return -1;
                }
                if (read(fd, &buf, sizeof(char)) == -1) {
                    return -1;
                }
                buf = buf | (1 << (var % 8));
                if (lseek(fd, var / 8, SEEK_SET) == -1) {
                    return -1;
                }
                if (write(fd, &buf, sizeof(char)) == -1) {
                    return -1;
                }
            }
        } else if (var < 0) {
            var = -var - 1;
            char buf;
            if (fstat.st_size > var / 8) {
                if (lseek(fd, var / 8, SEEK_SET) == -1) {
                    return -1;
                }
                if (read(fd, &buf, sizeof(char)) == -1) {
                    return -1;
                }
                buf = buf & (0xFF ^ (1 << (var % 8)));
                if (lseek(fd, var / 8, SEEK_SET) == -1) {
                    return -1;
                }
                if (write(fd, &buf, sizeof(char)) == -1) {
                    return -1;
                }
            }
        }
    }
    close(fd);
    return 0;
}
