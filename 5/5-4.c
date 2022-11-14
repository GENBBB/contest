#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <string.h>

struct Key
{
    struct stat f_stat;
    char mod;
};

int
cmp(const void *s1, const void *s2)
{
    return strcmp(*(char**) s1, *(char**)s2);
}

int
main(int argc, char *argv[])
{
    struct Key *rep_f = calloc(argc - 1, sizeof(*rep_f));
    qsort(argv + 1, argc - 1, sizeof(*argv), cmp);
    for (int i = 0; i < argc - 1; i++) {
        if (stat(argv[i + 1], &rep_f[i].f_stat) == -1) {
            rep_f[i].mod = 1;
        }
    }
    for (int i = argc - 1; i > 0; i--) {
        if (!rep_f[i].mod) {
            for (int j = i - 1; j >= 0; j--) {
                if (rep_f[j].f_stat.st_ino == rep_f[i].f_stat.st_ino) {
                    rep_f[j].mod = 1;
                }
            }
        }
    }
    for (int i = 1; i < argc; i++) {
        if (!rep_f[i-1].mod) {
            printf("%s\n", argv[i]);
        }
    }
    free(rep_f);
    return 0;
}
