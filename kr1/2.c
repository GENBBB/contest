#include <stdlib.h>
#include <errno.h>
#include <stdio.h>

int cmp(const void *a, const void *b) {
    if (*((unsigned long*)a) > *((unsigned long*)b)){
        return -1;
    } else if (*((unsigned long*)a) == *((unsigned long*)b)) {
        return 0;
    } else {
        return 1;
    }
}
int
main (int argc, char *argv[])
{
    unsigned long *arr = calloc(argc-1, sizeof (*arr));
    for (int i = 1; i < argc; i++) {
        char *eptr = NULL;
        errno = 0;
        unsigned long lval = strtoul(argv[i], &eptr, 17);
        if (errno || *eptr || eptr == argv[i]) {
            continue;
        } else {
            arr[i-1] = lval;
        }
    }
    qsort(arr,argc-1, sizeof (*arr), cmp);
    for (int i = 0; i < argc -1; i++) {
        printf("%lu\n", arr[i]);
    }
    free(arr);
    return 0;
}
