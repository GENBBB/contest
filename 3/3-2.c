#include <stdio.h>
#include <stdlib.h>
int 
main(int argc, char *argv[]) 
{
    long long int sum_pos = 0, sum_neg = 0;
    for (int i = 1; i < argc; i++) {
        int current = 0;
        if (!sscanf(argv[i], "%d", &current)) {
            fprintf(stderr, "Arguments error\n");
            exit(1);
        }
        if (current > 0) {
            if (__builtin_add_overflow(current, sum_pos, &sum_pos)) {
                fprintf(stderr, "Overflow error\n");
                exit(2);
            }
        } else {
            if (__builtin_add_overflow(current, sum_neg, &sum_neg)) {
                fprintf(stderr, "Overflow error\n");
                exit(2);
            }
        }
    }
    printf("%lld\n%lld\n", sum_pos, sum_neg);
    return 0;
}
