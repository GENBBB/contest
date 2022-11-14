#include <sys/stat.h>
#include <stdio.h>
int main(int argc, char *argv[]){
    off_t result = 0;
    for (int i = 1; i < argc; i++){
        struct stat buf;
        if (lstat(argv[i], &buf) != -1){
            if (buf.st_size % 1024 == 0) {
                if (S_ISREG(buf.st_mode) != 0 && S_ISLNK(buf.st_mode) == 0){
                    if (buf.st_nlink == 1){
                        result += buf.st_size;
                    }
                }
            }
        }    
    }
    printf("%lld\n", result);
}
