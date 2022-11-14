#include <stdio.h>
#include <stdlib.h>

int
main(void)
{
    int N;
    if (scanf("%d", &N) != 1) {
        fprintf(stderr, "Enter N\n");
        return 1;
    }

    for (int j = 1; j < N; ++j) {
        printf("0 ");
    }
    putchar('\n');

    int *division_row = calloc(N - 1, sizeof(int));
    for (int j = 1; j < N; ++j) {
        for (int k = 0; k < N; ++k) {
            if ((j * k) % N == 1) {
                division_row[j - 1] = k;
                printf("%d ", division_row[j - 1]);
                break;
            }
        }
    }
    putchar('\n');

    for (int i = 2; i < N; ++i) {
        for (int j = 1; j < N; ++j) {
            printf("%d ", division_row[j - 1] * i % N);
        }
        putchar('\n');
    }

    free(division_row);

    return 0;
}
