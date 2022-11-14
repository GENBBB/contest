#include <stdio.h>
#include <stdlib.h>

int
main (void)
{
    int sum_size = 512;
    int sum_len = 0;
    long *sum = calloc(sum_size, sizeof (*sum));
    long cur;
    while (scanf("%ld", &cur) == 1) {
        int flag_add = 0;
        for (int i = 0; i < sum_len; i++) {
            long tmp_sum = sum[i];
            if (__builtin_add_overflow(tmp_sum, cur, &tmp_sum)) {
                continue;
            } else {
                sum[i] = tmp_sum;
                flag_add = 1;
                break;
            }
        }
        if (!flag_add) {
            sum_len++;
            if (sum_len == sum_size) {
                sum_size *= 2;
                sum = reallocarray(sum, sum_size, sizeof (*sum));
            }
            sum[sum_len-1] = cur;
        }
    }
    if (sum_len == 0) {
        printf ("0\n");
        free(sum);
        return 0;
    }
    int *flags = calloc(sum_len, sizeof (*flags));
    int n_sum = sum_len;
    while (n_sum != 1) {
        for (int i = 1; i < sum_len; i++) {
            if (flags[i]) {
                continue;
            }
            long tmp_sum = sum[i];
            if (__builtin_add_overflow(tmp_sum, sum[0], &tmp_sum)) {
                continue;
            } else {
                sum[0] = tmp_sum;
                flags[i] = 1;
                n_sum--;
                break;
            }
        }
    }
    printf("%ld\n", sum[0]);
    free(flags);
    free(sum);
    return 0;
}
