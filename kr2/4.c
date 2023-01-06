#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

void
process(int n, int deep, int fd[2])
{
    if (n == deep) {
        int num;
        int end = scanf("%d", &num);
        if (end != EOF) {
            printf("%d %d\n", getpid(), num);
            fflush(stdout);
        } else {
            num = end;
        }
        if (write(fd[1], &num, sizeof(num)) != sizeof(num)) {
            _exit(1);
        }
    } else {
        if (!fork()) {
            process(n+1, deep, fd);
        }
        wait(NULL);
    }
    if (n > 1) {
        _exit(0);
    }
}

int
main(int argc, char *argv[]) {
    int fd[2];
    pipe(fd);
    int n;
    setbuf(stdin, NULL);
    scanf("%d", &n);
    printf("%d %d\n", getpid(), n);
    fflush(stdout);
    while (n != EOF) {
        process(1, n + 1, fd);
        if (read(fd[0], &n, sizeof(n)) != sizeof(n)) {
            return 1;
        }
    }
    return 0;
}
