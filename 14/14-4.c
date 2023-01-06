#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include <signal.h>

void
handler(int sig) {
    return;
}

void
process(int fd[2], int n, int p_num)
{
    sigaction(SIGUSR1, &(struct sigaction) {.sa_handler = handler, .sa_flags = SA_RESTART}, NULL);
    pid_t br_pid;
    FILE *in = fdopen(fd[0], "r");
    FILE *out = fdopen(fd[1], "w");
    if (p_num == 1) {
        if (fscanf(in, "%d", &br_pid) != 1) {
            fclose(in);
            fclose(out);
            _exit(1);
        }
        fprintf(out, "%d\n", getpid());
        fflush(out);
        kill(br_pid, SIGUSR1);
        pause();
        printf("1 1\n");
        fflush(stdout);
        fprintf(out, "1\n");
        fflush(out);
        kill(br_pid, SIGUSR1);
    } else if (p_num == 2) {
        fprintf(out, "%d\n", getpid());
        fflush(out);
        pause();
        if (fscanf(in, "%d", &br_pid) != 1) {
            fclose(in);
            fclose(out);
            _exit(1);
        }
        kill(br_pid, SIGUSR1);
    } else {
        fclose(in);
        fclose(out);
        _exit(1);
    }
    while (1) {
        unsigned long long count;
        pause();
        if (fscanf(in, "%llu", &count) != 1) {
            _exit(1);
        }
        if (count + 1 >= n) {
            fprintf(out, "%llu\n", count);
            fflush(out);
            fclose(out);
            fclose(in);
            kill(br_pid, SIGUSR1);
            _exit(0);
        }
        printf("%d %llu\n", p_num, count + 1);
        fflush(stdout);
        fprintf(out, "%llu\n", count + 1);
        fflush(out);
        kill(br_pid, SIGUSR1);
    }
}

int
main(int argc, char *argv[])
{
    if (argc < 1) {
        return 1;
    }
    int n;
    sscanf(argv[1], "%d", &n);
    if (n <= 1) {
        printf("Done\n");
        return 0;
    }
    int fd[2];
    if (pipe(fd) == -1) {
        return 1;
    }
    pid_t pid1 = fork();
    if (!pid1) {
        process(fd, n, 1);
    } else if (pid1 == -1) {
        return 1;
    }
    pid_t pid2 = fork();
    if (!pid2) {
        process(fd, n, 2);
    } else if (pid2 == -1) {
        return 1;
    }
    close(fd[0]);
    close(fd[1]);
    wait(NULL);
    wait(NULL);
    printf("Done\n");
    return 0;
}