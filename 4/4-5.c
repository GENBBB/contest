#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <limits.h>

enum
{
    FILE_MOD = 0666,
    ARGS_NUM = 4,
    ARG_OUT_FILE = 2,
    ARG_MOD = 3
};

int
main(int argc, char *argv[])
{
    int fd_in, fd_out;
    if (argc != ARGS_NUM) {
        return 1;
    }
    fd_in = open(argv[1], O_RDONLY);
    fd_out = open(argv[ARG_OUT_FILE], O_WRONLY | O_TRUNC | O_CREAT, FILE_MOD);
    if ((fd_in == -1) || (fd_out == -1)) {
        return 1;
    }
    char value = 0;
    int mod;
    if (sscanf(argv[ARG_MOD], "%d", &mod) != 1) {
        return 1;
    }
    long long sum = 0;
    int number_value = 0;
    while (read(fd_in, &value, sizeof(value)) == sizeof(value)) {
        int mask = 1;
        for (int i = 0; i < CHAR_BIT; i++) {
            number_value++;
            sum += (number_value % mod) * ((long long) number_value % mod) % mod;
            sum = sum % mod;
            if ((value & mask) != 0) {
                int result = sum;
                if (write(fd_out, &result, sizeof(result)) != sizeof(result)) {
                    return 1;
                }
            }
            mask = mask << 1;
        }
    }
    close(fd_out);
    close(fd_in);
    return 0;
}