#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
int 
main(int argc, char *argv[]) 
{
    char *eptr = NULL;
    long long sum_pos = 0, sum_neg = 0;
    for (int i = 1; i < argc; i++) {
        errno = 0;
        long lval = strtol(argv[i], &eptr, 10);
        if (errno || *eptr || (int) lval != lval) {
            fprintf(stderr, "wrong number of arguments\n");
            exit(1);
        }
        if (lval > 0) {
            if (__builtin_add_overflow(lval, sum_pos, &sum_pos)) {
                fprintf(stderr, "Overflow error\n");
                exit(2);
            }
        } else {
            if (__builtin_add_overflow(lval, sum_neg, &sum_neg)) {
                fprintf(stderr, "Overflow error\n");
                exit(2);
            }
        }
    }
    printf("%lld\n%lld\n", sum_pos, sum_neg);
    return 0;
}
