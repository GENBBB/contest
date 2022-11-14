#include <sys/stat.h>
#include <dirent.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

int
go_to_root(char *path)
{
    DIR *cur_dir;
    int len = 0;
    struct stat root_stat;
    if (lstat("/", &root_stat) == -1) {
        return -1;
    }
    struct stat cur_stat;
    if (lstat(".", &cur_stat) == -1) {
        return -1;
    }
    while (cur_stat.st_ino != root_stat.st_ino){
        if (chdir("..") == -1) {
            return -1;
        }
        cur_dir = opendir(".");
        struct dirent *file = NULL;
        while ((file = readdir(cur_dir)) != NULL) {
            struct stat f_stat;
            if (lstat(file->d_name, &f_stat) == -1) {
                return -1;
            }
            if (f_stat.st_ino == cur_stat.st_ino) {
                break;
            }
        }
        if (file == NULL) {
            return -1;
        }
        int name_len = strlen(file->d_name);
        memcpy(path + len, file->d_name, name_len);
        len += name_len;
        path[len] = '/';
        len++;
        if (lstat(".", &cur_stat) == -1) {
            return -1;
        }
        closedir(cur_dir);
    }
    if (len == 0) {
        path[0] = '/';
        path[1] = 0;
    } else {
        path[len] = 0;
    }
    return 0;
}

void
reverse_path(char *path)
{
    int len = strlen(path);
    char rev_path[PATH_MAX];
    int k = 0;
    for (int i = len - 1; i >= 0; i--) {
        if (path[i] == '/') {
            rev_path[k] = '/';
            k++;
        } else {
            while (path[i] != '/' && i != 0) {
                i--;
            }
            i = (i != 0) ? i + 1 : 0;
            for (int j = i; path[j] != '/'; j++) {
                rev_path[k] = path[j];
                k++;
            }
        }
    }
    memcpy(path, rev_path, len);
    return;
}
ssize_t
getcwd2(int fd, char *buf, size_t size)
{
    int init_fd = open(".", O_RDONLY);
    if (init_fd == -1) {
        return -1;
    }
    char res_dir[PATH_MAX];
    if (fchdir(fd) == -1) {
        return -1;
    }
    if (go_to_root(res_dir) == -1) {
        return -1;
    }
    reverse_path(res_dir);
    if (fchdir(init_fd) == -1) {
        return -1;
    }
    close(init_fd);
    if (size != 0) {
        memcpy(buf, res_dir, size - 1);
        buf[size - 1] = 0;
    }
    return strlen(res_dir);
}