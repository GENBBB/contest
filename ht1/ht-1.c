#include <sys/mman.h>
#include <fcntl.h>
#include <inttypes.h>
#include <stdio.h>
#include <unistd.h>

enum
{
    FILE_SIZE = 64 * 1024,
    SIZE_NUMBER_ADDR = 9,
    NUMBER_MASK = 0x1FF,
};


uint16_t
access_mem(uint16_t *file, uint16_t virtual_addr, uint16_t page_table)
{
    uint32_t page_addr = *(file + (page_table >> 1) + (virtual_addr >> SIZE_NUMBER_ADDR));
    return *(file + (page_addr >> 1) + ((virtual_addr & NUMBER_MASK) >> 1));
}

int
main(int argc, char *argv[])
{
    if (argc != 3) {
        return 1;
    }
    int fd = open(argv[1], O_RDONLY);
    if (fd == -1) {
        return 1;
    }
    uint16_t *file = mmap(NULL, FILE_SIZE,PROT_READ, MAP_PRIVATE , fd, 0);
    if (close(fd)) {
        return 1;
    }
    uint16_t page_table;
    sscanf(argv[2], "%hx", &page_table);
    uint16_t virtual_addr;
    while (scanf("%hx", &virtual_addr) == 1) {
        printf("%hu\n", access_mem(file, virtual_addr, page_table));
    }
    munmap(file, FILE_SIZE);
    return 0;
}