#include <dirent.h>
#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>

enum
{
    LEN_SUFFIX = 4
};

int
main(int argc, char *argv[])
{
    DIR *dir = opendir(argv[1]);
    const char * suffix = ".exe";
    struct dirent *file;
    char file_path[PATH_MAX];
    int result = 0;
    while ((file = readdir(dir)) != NULL) {
        int str_check = snprintf(file_path, PATH_MAX, "%s/%s", argv[1], file->d_name);
        if (str_check < 0  || str_check >= PATH_MAX) {
            return 1;
        }
        struct stat f_stat;
        if (stat(file_path, &f_stat) != 0) {
            continue;
        }
        if (S_ISREG(f_stat.st_mode) && access(file_path, X_OK) == 0) {
            int len = strlen(file->d_name);
            if (strcmp(&file->d_name[len - LEN_SUFFIX], suffix) == 0) {
                result++;
            }
        }

    }
    closedir(dir);
    printf("%d\n", result);
    return 0;
}
