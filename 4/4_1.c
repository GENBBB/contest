#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

enum
{
    CHAR_BIT = 8,
    SHORT_BIT_PENSIEVE = 24,
    SECOND_BYTE = 0xff00,
    THIRD_BYTE = 0xff0000,
    FOURTH_BYTE = 0xff000000,
    FIRST_BYTE_PENSIEVE = 0xfff,
    SECOND_BYTE_PENSIEVE = 0xfff000,
    SHIFT_PENSIEVE = 4,
    FILE_MOD = 0600
};
int
main (int argc, char *argv[])
{
    int fd = open(argv[1], O_CREAT | O_WRONLY | O_TRUNC, FILE_MOD);
    if (fd == -1) {
        return 1;
    }
    unsigned int buf = 0;
    unsigned int tmp = 0;
    while (scanf("%u", &tmp) == 1) {
        buf = (tmp & FIRST_BYTE_PENSIEVE) | ((tmp & SECOND_BYTE_PENSIEVE) << SHIFT_PENSIEVE);
        buf = (buf >> SHORT_BIT_PENSIEVE) | ((buf >> CHAR_BIT) & SECOND_BYTE) |
                ((buf << CHAR_BIT) & THIRD_BYTE) | ((buf << SHORT_BIT_PENSIEVE) & FOURTH_BYTE);
        if (write(fd, &buf, sizeof(buf)) != sizeof(buf)) {
            return 1;
        }
    }
    close(fd);
    return 0;
}
