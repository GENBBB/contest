#include <stdio.h>

int
main(void)
{
    unsigned prev, cur, res = 0;
    unsigned alignment = 1;
    if (scanf("%u", &prev) != 1) {
        printf("1 1\n");
        return 0;
    }
    res += prev;
    while (scanf("%u", &cur) == 1) {
        int div = 1;
        if (cur == 0) {
            continue;
        }
        while (!(div & cur)) {
            div *= 2;
        }
        int tmp = res % div;
        res += tmp ? div - tmp : 0;
        res += cur;
        if (div > alignment) {
            alignment = div;
        }
    }
    int tmp = res % alignment;
    res += tmp ? alignment - tmp : 0;
    printf("%u %u\n", res, alignment);
    return 0;
}
