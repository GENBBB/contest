#include <stdio.h>
#include <inttypes.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>

enum { RADIX = 10, SUFFIX = 1000 };

int32_t
parse(char *str, char *operand)
{
    char mul_flag = 0;
    size_t len = strlen(str);
    *operand = str[len - 1];
    if (*operand != '+' && *operand != '-') {
        return  0;
    }
    str[len - 1] = 0;
    len--;
    if (len > 0) {
        if (str[len - 1] == 'k') {
            mul_flag = 1;
            str[len - 1] = 0;
        }
    }
    char *eptr = NULL;
    errno = 0;
    long lval = strtol(str, &eptr, RADIX);
    if (errno || eptr == str || *eptr || (int32_t) lval != lval) {
        return 0;
    }
    int32_t result = lval;
    if (mul_flag && __builtin_mul_overflow(result, SUFFIX, &result)) {
        return 0;
    }
    return result;
}

int
main(int argc, char *argv[])
{
    long long result = 0;
    for (int i = 1; i < argc; i++) {
        char operand;
        int32_t term = parse(argv[i], &operand);
        if (operand == '+') {
            result += term;
        } else {
            result -= term;
        }
    }
    printf("%lld\n", result);
    return 0;
}
