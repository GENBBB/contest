#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
int
main (int argc, char *argv[])
{
    int fd = open(argv[1], O_RDWR);
    int n;
    sscanf(argv[2], "%d", &n);
    int *arr = calloc(n, sizeof(*arr));
    n = read(fd, arr, 4*n) / 4;
    for (int i = 0; i < n; i++){
        printf("%d ", arr[i]);
    }
    printf("\n");
    return 0;
}
