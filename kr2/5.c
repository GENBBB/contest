#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>

void process(char *proc, int *fd_in, int *fd_out) {
    if (fd_in != NULL) {
        if (dup2(*fd_in ,STDIN_FILENO) == -1) {
            _exit(1);
        }
        close(*fd_in);
    }
    if (fd_out != NULL) {
        if (dup2(*fd_out ,STDOUT_FILENO) == -1) {
            _exit(1);
        }
        close(*fd_out);
    }
    execlp(proc, proc, NULL);
    _exit(1);
}

int
main(int argc, char *argv[])
{
    int fd[2];
    if (pipe(fd) == -1) {
        return 1;
    }
    pid_t pid1 = fork();
    if (!pid1) {
        close(fd[0]);
        int file = open(argv[2], O_RDONLY);
        process(argv[1], &file, &fd[1]);
    } else if (pid1 == -1) {
        return 1;
    }
    pid_t pid2 = fork();
    if (!pid2) {
        close(fd[1]);
        process(argv[3], &fd[0], NULL);
    } else if (pid2 == -1) {
        return 1;
    }
    close(fd[0]);
    close(fd[1]);
    int status1, status2;
    wait(&status1);
    wait(&status2);
    if (WIFEXITED(status1) && !WEXITSTATUS(status1) && WIFEXITED(status2) && !WEXITSTATUS(status2)) {
        if (!fork()) {
            process(argv[4], NULL, NULL);
        }
    }
    wait(NULL);
    return 0;
}