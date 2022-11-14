#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
int
main(int argc, char *argv[])
{
    if (argc != 4) {
        return 1;
    }
    int fd_in = open(argv[1], O_RDONLY);
    int fd_out = open(argv[2], O_WRONLY | O_TRUNC | O_CREAT, 0666);
    if ((fd_in == -1) || (fd_out == -1)){
        return 1;
    }
    char value = 0;
    int mod;
    if (sscanf(argv[3], "%d", &mod) != 1) {
        return 1;
    }
    long long sum = 0;
    int number_value = 0;
    while(read(fd_in, &value, 1)) {
        int mask = 1;
        for (int i = 0; i < 8; i++) {
            number_value++;
            sum += ((number_value%mod) * ((long long)number_value%mod)) % mod;
            sum = sum % mod;
            if ((value & mask) != 0) {
                int result = (int) sum;
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
