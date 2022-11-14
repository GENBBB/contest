#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include<sys/stat.h>
#include<fcntl.h>
int
main (int argc, char *argv[])
{
    int fd = open(argv[1], O_RDWR | O_CREAT | O_TRUNC, 0666);
    int n;
    sscanf(argv[2], "%d", &n);
    double *arr = calloc(n, sizeof(*arr));
    for (int i = 0; i < n; i++){
        scanf("%lf", arr+i);
    }
    if (write(fd, arr, sizeof(double)*n) != sizeof(double) * n) {
        printf("Gavno\n");
    }
    return 0;
}
