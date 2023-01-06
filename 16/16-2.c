#include <stdint.h>
#include <stdio.h>

int64_t
calc_max_size(int64_t block_size, int64_t block_num_size, int64_t inode_direct_block_count)
{
    int64_t res = inode_direct_block_count;
    int64_t tmp = block_size/block_num_size;
    res += tmp + tmp * tmp + tmp * tmp * tmp;
    if (__builtin_mul_overflow(res, block_size, &res)){
        return -1;
    }
    return res;
}

int
main(void)
{
    int64_t block_size;
    int64_t block_num_size;
    int64_t inode_direct_block_count;
    scanf("%lld%lld%lld", &block_size, &block_num_size, &inode_direct_block_count);
    printf("%lld\n", calc_max_size(block_size, block_num_size, inode_direct_block_count));
    for (int i = 0; i < 5; i++) {
        printf("%lld\n", calc_max_size(block_size, block_num_size-2+i, inode_direct_block_count));
    }
}