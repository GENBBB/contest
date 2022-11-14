#include <fcntl.h>
#include <limits.h>
#include <unistd.h>
int
main(int argc, char *argv[])
{
    int fd = open(argv[1], O_RDWR);
    if (fd == -1) {
        return 1;
    }
    long long min, buf;
    int id = 0, cur_id = 0;
    if (read(fd, &min, sizeof(min)) != sizeof(min)) {
        close(fd);
        return 0;
    }
    while (read(fd, &buf, sizeof(buf)) == sizeof(buf)) {
        cur_id++;
        if (buf < min) {
            min = buf;
            id = cur_id;
        }
    }
    if (min == LLONG_MIN) {
        close(fd);
        return 0;
    }
    min = -min;
    if (lseek(fd, sizeof(min) * id, SEEK_SET) == -1) {
        return 1;
    }
    if (write(fd, &min, sizeof(min)) != sizeof(min)) {
        return 1;
    }
    close(fd);
    return 0;
}
