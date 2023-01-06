#include <stdio.h>
#include <stdlib.h>

int
get_size(int first_block, int N, int *FAT) {
    int res = 0;
    while (first_block != 0 && first_block != N - 1 && first_block != N - 2){
        res++;
        first_block = FAT[first_block];
    }
    return res;
}

int
main(void)
{
    int N;
    scanf("%d",&N);
    int *FAT = calloc(N, sizeof(int));
    for (int i = 2; i < N - 2; i++) {
        scanf("%d", FAT + i);
    }
    int first_block;
    while (scanf("%d", &first_block) == 1) {
        printf("%d\n", get_size(first_block, N, FAT));
    }
    free(FAT);
    return 0;
}