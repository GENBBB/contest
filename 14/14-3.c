#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>
#include <sys/wait.h>
#include <string.h>

int
main(int argc, char *argv[])
{
    if (argc < 2) {
        return 1;
    }
    int n, result = 0;
    sscanf(argv[1], "%d", &n);
    for (int i = 2; i < argc && i - 2 < n; i++) {
        pid_t pid = fork();
        if (!pid) {
            FILE *file = fopen(argv[i], "r");
            if (file == NULL) {

                _exit(1);
            }
            char exe_path[PATH_MAX + 1];
            if (fgets(exe_path, PATH_MAX, file) == NULL) {
                _exit(1);
            }
            fclose(file);
            exe_path[strlen(exe_path) - 1] = 0;
            execlp(exe_path, exe_path, NULL);
            _exit(1);
        } else if (pid == -1) {
            return 1;
        }
    }
    int status;
    while (wait(&status) > 0) {
        result += (WIFEXITED(status) && !WEXITSTATUS(status));
    }
    for (int i = 2 + n; i < argc; i++) {
        pid_t pid = fork();
        if (!pid) {
            FILE *file = fopen(argv[i], "r");
            if (file == NULL) {
                _exit(1);
            }
            char exe_path[PATH_MAX + 1];
            if (fgets(exe_path, PATH_MAX, file) == NULL) {
                _exit(1);
            }
            fclose(file);
            exe_path[strlen(exe_path) - 1] = 0;
            execl(exe_path, exe_path, NULL);
            _exit(1);
        } else if (pid == -1) {
            return 1;
        }
        while (wait(&status) > 0) {
            result += (WIFEXITED(status) && !WEXITSTATUS(status));
        }
    }
    printf("%d\n", result);
    fflush(stdout);
    return 0;
}