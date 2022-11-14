#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

enum
{
    REC_DEEP = 3,
    START_SIZE = 32,
    N_ARG_SIZE = 2
};

void
arr_free(char **arr, int len)
{
    for (int i = 0; i < len; i++) {
        free(arr[i]);
    }
    free(arr);
}

void 
traversal(char *path, int deep, off_t max_fsize, int shift)
{
    char **dirs;
    DIR *dir;
    if (deep > REC_DEEP) {
        return;
    }
    dir = opendir(path);
    if (dir == NULL) {
        return;
    }
    char file_path[PATH_MAX];
    int dirs_size = START_SIZE;
    dirs = calloc(dirs_size, sizeof(*dirs));
    int dirs_len = 0;
    struct dirent *file; 
    while ((file = readdir(dir)) != NULL) {
        if (strcmp(file->d_name, ".") != 0  && strcmp(file->d_name, "..") != 0) {
            snprintf(file_path, PATH_MAX, "%s/%s", path, file->d_name);
            struct stat f_stat;
            if (lstat(file_path, &f_stat) != -1) {
                if (S_ISLNK(f_stat.st_mode)) {
                    continue;
                } else if (S_ISDIR(f_stat.st_mode)) {
                    if (dirs_len == dirs_size) {
                        dirs_size <<= 1;
                        dirs = reallocarray(dirs, dirs_size, sizeof(*dirs));
                    }
                    dirs[dirs_len] = calloc(PATH_MAX, sizeof(**dirs));
                    memcpy(dirs[dirs_len], file_path, PATH_MAX);
                    dirs_len++;
                } else if (!access(file_path, R_OK) && f_stat.st_size <= max_fsize && S_ISREG(f_stat.st_mode)) {
                    printf("%s\n", file_path + shift + 1);
                }
            }
        }
    }
    closedir(dir);
    for (int i = 0; i < dirs_len; i++) {
        traversal(dirs[i], deep + 1, max_fsize, shift);
    }
    arr_free(dirs, dirs_len);
    return;
}

int 
main(int argc, char *argv[])
{
    off_t max_fsize;
    sscanf(argv[N_ARG_SIZE], "%lld", &max_fsize);
    int shift = strlen(argv[1]);
    traversal(argv[1], 0, max_fsize, shift);
    return 0;
}