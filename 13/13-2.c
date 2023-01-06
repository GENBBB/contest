#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>

long long
child_process(int fd)
{
    long long result = 0;
    int  n;
    while (read(fd, &n, sizeof(n)) == sizeof(n)) {
        result += n;
    }
    return result;
}

int
main(void)
{
    int fd[2];
    if (pipe(fd) == -1) {
        return 1;
    }
    pid_t ch1 = fork();
    if (!ch1) {
        pid_t ch2 = fork();
        if (!ch2) {
            close(fd[1]);
            long long result = child_process(fd[0]);
            printf("%lld\n", result);
            fflush(stdout);
            _exit(0);
        } else if (ch2 == -1) {
            _exit(1);
        }
        close(fd[0]);
        close(fd[1]);
        wait(NULL);
        _exit(0);
    } else if (ch1 == -1) {
        return 1;
    }
    close(fd[0]);
    int num;
    while (scanf("%d", &num) == 1) {
        if (write(fd[1], &num, sizeof(num)) != sizeof(num)){
            return 1;
        }
    }
    close(fd[1]);
    wait(NULL);
    return 0;
}
