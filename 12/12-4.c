#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <limits.h>
#include <stdio.h>
#include <string.h>



int
main(int argc, char *argv[])
{
    if (argc < 2) {
        return 1;
    }
    char *tmp_path;
    if (!(tmp_path = getenv("XDG_RUNTIME_DIR"))) {
        if (!(tmp_path = getenv("TMPDIR"))) {
            tmp_path = "/tmp";
        }
    }
    char file_path[PATH_MAX];
    pid_t pid = getpid();
    sprintf(file_path, "%s/%d", tmp_path, pid);
    int fd = open(file_path, O_CREAT | O_WRONLY | O_TRUNC, 0700);
    if (fd == -1) {
        return 1;
    }
    const char *scr_begin = "#!/bin/python3\nimport os\n";
    if (write(fd, scr_begin, strlen(scr_begin)) != strlen(scr_begin)) {
        return 1;
    }
    const char *a_init = "a = ";
    if (write(fd, a_init, strlen(a_init)) != strlen(a_init)) {
        return 1;
    }
    if (write(fd, argv[1], strlen(argv[1])) != strlen(argv[1])) {
        return 1;
    }
    if (write(fd, "\n", 1) != 1) {
        return 1;
    }
    const char *a_mul = "a *=";
    for (int i = 2; i < argc; i++) {
        if (write(fd, a_mul, strlen(a_mul)) != strlen(a_mul)) {
            return 1;
        }
        if (write(fd, argv[i], strlen(argv[i])) != strlen(argv[i])) {
            return 1;
        }
        if (write(fd, "\n", 1) != 1) {
            return 1;
        }
    }
    const char *print = "print(a)\n";
    if (write(fd, print, strlen(print)) != strlen(print)) {
        return 1;
    }
    const char *scr_con = "os.remove(\"";
    if (write(fd, scr_con, strlen(scr_con)) != strlen(scr_con)) {
        return 1;
    }
    if (write(fd, file_path, strlen(file_path)) != strlen(file_path)) {
        return 1;
    }
    const char *scr_end = "\")";
    if (write(fd, scr_end, strlen(scr_end)) != strlen(scr_end)) {
        return 1;
    }
    close(fd);
    execl("/bin/bash", "bash", "-c", file_path, NULL);
    return 1;
}