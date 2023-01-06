#define _GNU_SOURCE
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>

void
process (int fd_in, int fd_out, int n, int p_num)
{
    FILE *in = fdopen(fd_in, "r");
    FILE *out = fdopen(fd_out, "w");
    if (p_num == 1) {
        printf("1 1\n");
        fflush(stdout);
        fprintf(out, "1\n");
        fflush(out);
    }
    while (1) {
        unsigned long long count;
        if (fscanf(in, "%llu", &count) != 1) {
            fclose(out);
            fclose(in);
            _exit(0);
        }
        if (count + 1 == n) {
            fclose(out);
            fclose(in);
            _exit(0);
        }
        printf("%d %llu\n", p_num, count + 1);
        fflush(stdout);
        fprintf(out, "%llu\n", count + 1);
        fflush(out);
    }
}

int
main(int argc, char *argv[])
{
    int n;
    sscanf(argv[1], "%d", &n);
    if (n <= 1) {
        printf("Done\n");
        return 0;
    }
    int to_second[2];
    int to_first[2];
    if (pipe(to_second) == -1) {
        return 1;
    }
    if (pipe(to_first) == -1) {
        return 1;
    }
    pid_t pid1 = fork();
    if (!pid1) {
        close(to_first[0]);
        close(to_second[1]);
        process(to_second[0], to_first[1], n, 1);
    } else if (pid1 == -1) {
        return 1;
    }
    pid_t pid2 = fork();
    if (!pid2) {
        close(to_first[1]);
        close(to_second[0]);
        process(to_first[0], to_second[1], n, 2);
    } else if (pid2 == -1) {
        return 1;
    }
    close(to_first[0]);
    close(to_first[1]);
    close(to_second[0]);
    close(to_second[1]);
    wait(NULL);
    wait(NULL);
    printf("Done\n");
    return 0;
}