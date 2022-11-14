#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <string.h>

void
fill_matrix(int *matrix, size_t rows, size_t cols)
{
    int k = 1, j1 = 0, i1 = 0, q = 0, stop = 0;
    while(k <= rows * cols){
        for(int j = j1; j < cols - q; j++){
            if (k > rows * cols){
                stop = 1;
                break;
            }
            matrix[i1 * cols + j] = k;
            k++;
            j1++;
        }
        j1--;
        i1++;
        if (stop) {
            break;
        }
        for(int i = i1; i < rows - q; i++){
            if (k > rows * cols){
                stop = 1;
                break;
            }
            matrix[i * cols + j1] = k;
            k++;
            i1++;
        }
        i1--;
        j1--;
        if (stop) {
            break;
        }
        for(int j = j1; j >= q; j--){
            if (k > rows * cols){
                stop = 1;
                break;
            }
            matrix[i1 * cols + j] = k;
            k++;
            j1--;
        }
        j1++;
        i1--;
        if (stop) {
            break;
        }
        for(int i = i1; i > q; i--){
            if (k > rows * cols){
                stop = 1;
                break;
            }
            matrix[i * cols + j1] = k;
            k++;
            i1--;
        }
        i1++;
        j1++;
        if (stop) {
            break;
        }
        q++;
    }
}

int
main(int argc, char *argv[])
{
    if (argc != 4) {
        return 1;
    }
    size_t rows, cols;
    if (sscanf(argv[2], "%d", &rows) != 1) {
        return 1;
    }
    if (sscanf(argv[3], "%d", &cols) != 1) {
        return 1;
    }
    int fd = open(argv[1], O_RDWR | O_CREAT | O_TRUNC, 0666);
    if (fd == -1) {
        return 1;
    }
    if (ftruncate(fd, 4 * rows * cols) == -1) {
        return 1;
    }
    void *file = mmap(0, 4 * rows * cols, PROT_WRITE, MAP_SHARED, fd, 0);
    if (file == MAP_FAILED) {
        return 1;
    }
    int *matrix = file;
    fill_matrix(matrix, rows, cols);
    munmap(matrix, rows * cols);
    close(fd);
    return 0;
}