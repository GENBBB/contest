#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>

struct Node
{
    int32_t key;
    int32_t left_idx;
    int32_t right_idx;
};

enum
{
    OPEN_ERROR = 3,
    SEEK_ERROR = 2
};

void 
out_branch(int fd, int id) 
{
    struct Node current;
    if (lseek(fd, sizeof(current) * id, SEEK_SET) == -1) {
        fprintf(stderr, "Error!\n");
        exit(SEEK_ERROR);
    }
    if (read(fd, &current, sizeof(current)) != sizeof(current)) {
        fprintf(stderr, "Error!\n");
        exit(1);
    }
    if (current.right_idx != 0) {
        out_branch(fd, current.right_idx);
    }
    printf("%d ", current.key);
    if (current.left_idx != 0) {
        out_branch(fd, current.left_idx);
    }
}

int
main(int argc, char *argv[])
{
    int fd = open(argv[1], O_RDONLY);
    if (fd == -1) {
        return OPEN_ERROR;
    }
    out_branch(fd, 0);
    printf("\n");
    close(fd);
    return 0;
}
