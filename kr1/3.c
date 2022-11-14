#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

void
arr_free(char ** arr, int len)
{
    for (int i = 0; i < len; i++) {
        free(arr[i]);
    }
    free(arr);
}

void
traversal(char *path, off_t *max_fsize, uid_t uid)
{
    DIR *dir = opendir(path);
    if (dir == NULL) {
        return;
    }
    char file_path[PATH_MAX];
    int dirs_size = 32;
    char ** dirs = calloc(dirs_size, sizeof(*dirs));
    int dirs_len = 0;
    struct dirent *file;
    while ((file = readdir(dir)) != NULL) {
        if (!strcmp(file->d_name, ".") || !strcmp(file->d_name, "..")) {
            continue;
        }
        snprintf(file_path, PATH_MAX, "%s/%s", path, file->d_name);
        struct stat f_stat;
        if (lstat(file_path, &f_stat) != -1) {
            if (S_ISDIR(f_stat.st_mode)) {
                if (dirs_len == dirs_size) {
                    dirs_size <<= 1;
                    dirs = reallocarray(dirs, dirs_size, sizeof(*dirs));
                }
                dirs[dirs_len] = calloc(PATH_MAX, sizeof(**dirs));
                memcpy(dirs[dirs_len], file_path, PATH_MAX);
                dirs_len++;
            } else if (S_ISREG(f_stat.st_mode) && f_stat.st_uid == uid) {
                *max_fsize += f_stat.st_size;
            }
        }
    }
    closedir(dir);
    for (int i = 0; i < dirs_len; i++) {
        traversal(dirs[i], max_fsize, uid);
    }
    arr_free(dirs, dirs_len);
    return;
}

int
main(int argc, char *argv[])
{
    off_t max_fsize = 0;
    uid_t uid;
    sscanf(argv[2], "%d", &uid);
    traversal(argv[1], &max_fsize, uid);
    printf("%lld\n", max_fsize);
    return 0;
}
