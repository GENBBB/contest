#include <dirent.h>
#include <sys/stat.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

int
main (int argc, char *argv[])
{
    if (argc != 3) {
        return 2;
    }
    DIR * dir = opendir(argv[1]);
    if (dir == NULL) {
        return 1;
    }
    struct dirent *file;
    off_t result = 0;
    while ((file = readdir(dir)) != NULL) {
        char file_path[PATH_MAX];
        if (!strcmp(file->d_name, ".") || !strcmp(file->d_name, "..")) {
            continue;
        }
        snprintf(file_path, PATH_MAX, "%s/%s", argv[1], file->d_name);
        struct stat f_stat;
        if (lstat(file_path, &f_stat) != -1) {
            if (S_ISLNK(f_stat.st_mode)) {
                continue;
            } else if (S_ISREG(f_stat.st_mode) && !access(file_path, W_OK)) {
                char  c_path[PATH_MAX];
                struct stat c_stat;
                snprintf(c_path, PATH_MAX, "%s/%s", argv[2], file->d_name);
                if (stat(c_path, &c_stat) != -1) {
                    if (c_stat.st_ino == f_stat.st_ino) {
                        result += f_stat.st_size;
                    }
                }
            }
        }
    }
    closedir(dir);
    printf("%lld\n", result);
    return 0;
}
