#include <sys/stat.h>
#include <stddef.h>
#include <stdio.h>
#include <unistd.h>
#include <limits.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>

enum
{
    MAX_CPBUF = 4 * 1024,
    ALL_RIGHT = 0777
};

int
copy_file(const char *srcpath, const char *dstpath)
{
    int fd_src = open(srcpath, O_RDONLY);
    int fd_dst = open(dstpath, O_CREAT | O_EXCL, ALL_RIGHT);
    if (fd_src == -1) {
        close(fd_src);
        return -1;
    }
    struct stat dst_stat;
    struct stat src_stat;
    if ((stat(srcpath, &src_stat) == -1)) {
        close(fd_src);
        return -1;
    }
    int f_mode = src_stat.st_mode & ALL_RIGHT;
    if (fd_dst == -1) {
        errno = 0;
        if ((stat(dstpath, &dst_stat) == -1)) {
            close(fd_src);
            return -1;
        }
        if (dst_stat.st_ino == src_stat.st_ino) {
            return 0;
        }
    } else {
        close(fd_dst);
    }
    if (S_ISDIR(dst_stat.st_mode)) {
        char path[PATH_MAX];
        const char *fname = srcpath;
        for (int i = strlen(srcpath); i >= 0; i--) {
            if (srcpath[i] == '/') {
                fname = srcpath + i + 1;
                break;
            }
        }
        snprintf(path, PATH_MAX, "%s/%s", dstpath, fname);
        fd_dst = open(path, O_WRONLY | O_CREAT | O_TRUNC, f_mode);
    } else {
        fd_dst = open(dstpath, O_WRONLY | O_CREAT | O_TRUNC, f_mode);
    }
    if (fd_dst == -1) {
        close(fd_src);
        return -1;
    }
    int size_r;
    char buf[MAX_CPBUF];
    while ((size_r = read(fd_src, buf, MAX_CPBUF)) > 0) {
        int size_w = 0;
        while (size_w != size_r) {
            int size_tmp = write(fd_dst, buf + size_w, size_r - size_w);
            if (size_tmp == -1) {
                close(fd_src);
                close(fd_dst);
                return -1;
            }
            size_w += size_tmp;
        }
    }
    if (size_r == -1) {
        close(fd_src);
        close(fd_dst);
        return -1;
    }
    fchmod(fd_dst, f_mode);
    close(fd_src);
    close(fd_dst);
    return 0;
}