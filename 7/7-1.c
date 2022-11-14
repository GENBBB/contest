#include <stdio.h>
#include <inttypes.h>
#include <stdint.h>
#include <limits.h>
#include <ctype.h>

enum
{
    RADIX = 3,
    BUF_SIZE = 512
};

int
main(int argc, char *argv[])
{
    long long MIN_BORDER = -3074457345618258603;
    const char *ERROR_STR = "18446744073709551616";
    int symbol;
    char new_flag = 0;
    int64_t result = 0;
    while ((symbol = getchar()) != EOF) {
        if (isspace(symbol)) {
            if (new_flag) {
                printf("%" SCNd64 "\n", result);
                new_flag = 0;
                result = 0;
            }
            continue;
        }
        new_flag = 1;
        if (result == MIN_BORDER && symbol == '1') {
            result = LLONG_MIN;
            continue;
        }
        if (__builtin_mul_overflow(result, RADIX, &result)) {
            printf("%s\n", ERROR_STR);
            result = 0;
            new_flag = 0;
            while (isspace(symbol) == 0 && symbol != EOF) {
                symbol = getchar();
            }
            if (symbol == EOF) {
                break;
            }
            continue;
        }
        if (symbol == 'a') {
            result--;
        } else if (symbol == '1' || symbol == '0') {
            result += symbol - '0';
        } else {
            printf("%s\n", ERROR_STR);
            new_flag = 0;
            result = 0;
            while (isspace(symbol) == 0 && symbol != EOF) {
                symbol = getchar();
            }
            if (symbol == EOF) {
                break;
            }
        }
    }
    if (new_flag) {
        printf("%" SCNd64 "\n", result);
    }
    return 0;
}
