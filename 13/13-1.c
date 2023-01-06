#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>
#include <sys/wait.h>

void
child_process(int fd)
{
    time_t t = time(NULL);
    for (int  i = 0; i < 3; i++) {
        if (write(fd, &t, sizeof(t)) != sizeof(t)){
            printf("aaa\n");
            fflush(stdout);
            _exit(1);
        }
    }
    _exit(0);
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
            pid_t ch3 = fork();
            if (!ch3) {
                close(fd[0]);
                child_process(fd[1]);
            } else if (ch3 == -1) {
                return 1;
            }
            wait(NULL);
            close(fd[1]);
            time_t t;
            if (read(fd[0], &t, sizeof(t)) != sizeof(t)){
                _exit(1);
            }
            struct tm *time = localtime(&t);
            printf("D:%d\n", time->tm_mday);
            fflush(stdout);
            _exit(0);
        } else if (ch2 == -1) {
            return 1;
        }
        wait(NULL);
        close(fd[1]);
        time_t t;
        if (read(fd[0], &t, sizeof(t)) != sizeof(t)){
            _exit(1);
        }
        struct tm *time = localtime(&t);
        printf("M:%d\n", time->tm_mon+1);
        fflush(stdout);
        _exit(0);
    } else if (ch1 == -1) {
        return 1;
    }
    wait(NULL);
    close(fd[1]);
    time_t t;
    if (read(fd[0], &t, sizeof(t)) != sizeof(t)){
        _exit(1);
    }
    struct tm *time = localtime(&t);
    printf("Y:%d\n", time->tm_year+1900);
    fflush(stdout);
    return 0;
}