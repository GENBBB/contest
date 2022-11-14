#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include<sys/stat.h>
#include<fcntl.h>
int
main (int argc, char *argv[])
{
    int fd = open(argv[1], O_RDWR);
    int n;
    sscanf(argv[2], "%d", &n);
    double *arr = calloc(n, sizeof(*arr));
    read(fd, arr, 8*n);
    for (int i = 0; i < n; i++){
        printf("%lf\n", arr[i]);
    }
    return 0;
}
