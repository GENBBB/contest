#include <stdlib.h>

int
cmp(const int32_t *a, const int32_t *b)
{
    if ((*a & 1) != (*b & 1)) {
        return ((*a & 1) == 0) ? -1 : 1;
    } else if ((*a & 1) == 0) {
        if (*a == *b) {
            return 0;
        }
        return (*a > *b) ? 1 : -1;
    } else {
        if (*a == *b) {
            return 0;
        }
        return (*a > *b) ? -1 : 1;
    }
}

void
sort_even_odd(size_t count, int *data)
{
    qsort(data, count, sizeof(*data), (int(*) (const void *, const void *)) cmp);
}
