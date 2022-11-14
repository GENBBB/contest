#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <inttypes.h>

enum
{
    SHIFT = 2,
    MEMORY_ERROR = 2
};

void 
swap(uint32_t *a, uint32_t *b)
{
    uint32_t c = *a;
    *a = *b;
    *b = c;
}

uint32_t 
fact(uint32_t n)
{
    if (n == 0 || n == 1) {
        return 1;
    }
    return n * fact(n - 1);
}

void
next_permutation(uint32_t *arr, uint32_t n)
{   
    size_t need_swap = n - SHIFT;
    for (size_t i = n - SHIFT; i >= 0; i--) {
        if (arr[i] < arr[i + 1]) {
            need_swap = i;
            break;
        }
    }
    for (size_t i = n - 1; i > 0; i--) {
        if (arr[i] > arr[need_swap]) {
            swap(arr + i, arr + need_swap);
            break;
        }
    }
    uint32_t border = (n - need_swap - 1) >> 1;
    for (uint32_t i = 1; i <= border; i++) {
        swap(arr + need_swap + i, arr + n - i);
    }
}

void
array_print(uint32_t *arr, uint32_t n)
{
    for (size_t i = 0; i < n; i++) {
        printf("%d", arr[i]);
    }
    putchar('\n');
}

int
main(void)
{
    uint32_t n;
    if (scanf("%" SCNd32, &n) != 1) {
        fprintf(stderr, "Input error!\n");
        return 1;
    }
    uint32_t arr[10];
    for (size_t i = 0; i < n; i++) {
        arr[i] = i + 1;
    }
    uint32_t number_permutation = fact(n);
    array_print(arr, n);
    for (uint32_t i = 0; i < number_permutation - 1; i++) {
        next_permutation(arr, n);
        array_print(arr, n);
    }
    return 0;
}
