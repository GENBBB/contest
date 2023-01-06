#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>

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

void
and_proc(int fd, char *proc1, char *proc2, char *file1)
{
    if (dup2(fd ,STDOUT_FILENO) == -1) {
        _exit(1);
    }
    close(fd);
    pid_t pid1 = fork();
    if (!pid1) {
        int fd_in = open(file1, O_RDONLY);
        if (fd_in == -1) {
            _exit(1);
        }
        process(proc1, &fd_in, NULL);
    } else if (pid1 == -1) {
        _exit(1);
    }
    int status;
    wait(&status);
    if (WIFEXITED(status) && !WEXITSTATUS(status)) {
        int pid2 = fork();
        if (!pid2) {
            process(proc2, NULL, NULL);
        } else if (pid2 == -1) {
            _exit(1);
        }
    }
    wait(NULL);
    _exit(0);
}

void
or_proc(int fd, char *proc, char *file) {
    if (dup2(fd, STDIN_FILENO) == -1) {
        _exit(1);
    }
    close(fd);
    pid_t pid3 = fork();
    if (!pid3) {
        int fd_file = open(file, O_WRONLY | O_CREAT | O_APPEND, 0666);
        process(proc, NULL, &fd_file);
    } else if (pid3 == -1) {
        _exit(1);
    }
    wait(NULL);
    _exit(0);
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
        and_proc(fd[1], argv[1], argv[2],argv[4]);
    } else if (pid1 == -1) {
        return 1;
    }

    pid_t pid2 = fork();
    if (!pid2) {
        close(fd[1]);
        or_proc(fd[0], argv[3], argv[5]);
    } else if (pid2 == -1) {
        return 1;
    }
    close(fd[0]);
    close(fd[1]);
    wait(NULL);
    wait(NULL);
    return 0;
}