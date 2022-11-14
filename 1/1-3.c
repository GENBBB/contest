#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>

enum { MAX_SIZE = 2000 };

int32_t 
extended_euclid (int32_t a, int32_t b, int32_t *x, int32_t *y)
{
    if (a == 0) {
        *x = 0; 
        *y = 1;
        return b;
    }
    int32_t x1, y1;
    int32_t d = extended_euclid(b % a, a, &x1, &y1);
    *x = y1 - (b / a) * x1;
    *y = x1;
    return d;
}

int
main(void)
{
    int32_t character;
    if (scanf("%" SCNd32, &character) != 1) {
        fprintf(stderr, "Input error!\n");
        return 1;
    }
    int32_t inverse_elements[MAX_SIZE];
    for (int32_t i = 1; i < character; i++) {
        int32_t inverse, y;
        int32_t gcd_m_i = extended_euclid(i, character, &inverse, &y);
        if (gcd_m_i != 1) {
            fprintf(stderr, "Entered non-prime number!\n");
            return 3;
        }
        inverse_elements[i] = (inverse % character + character) % character;
    }
    for (int32_t i = 0; i < character; i++) {
        for (int32_t j = 1; j < character; j++) {
            printf("%" SCNd32 " ", inverse_elements[j] * i % character);
        }
        putchar('\n');
    }
    return 0;
}  
