#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef struct Vertex
{
    int32_t pre_sum;
    int32_t value;
} Vertex;

int32_t min(int32_t a, int32_t b) {
    return a > b ? b : a;
}

int32_t max(int32_t a, int32_t b) {
    return a > b ? a : b;
}

int32_t 
get(
        Vertex * segment_tree, 
        int32_t ver, 
        int32_t range_left, 
        int32_t range_right, 
        int32_t req_left, 
        int32_t req_right)
{
    if (req_left > req_right) {
        return 0;
    }
    if (range_left == range_right) {
        return segment_tree[ver].value;
    }
    if (range_left == req_left && req_right == range_right) {
        return segment_tree[ver].value * (range_right - range_left + 1) + segment_tree[ver].pre_sum;
    }
    segment_tree[ver << 1].value += segment_tree[ver].value;
    segment_tree[(ver << 1) + 1].value += segment_tree[ver].value;
    segment_tree[ver].value = 0;
    int32_t range_mid = (range_left + range_right) >> 1;
    segment_tree[ver].pre_sum = segment_tree[ver << 1].value * (range_mid - range_left + 1) 
            + segment_tree[ver << 1].pre_sum
            + segment_tree[(ver << 1) + 1].value * (range_right - range_mid) + segment_tree[(ver << 1) + 1].pre_sum;
    
    return get(segment_tree, ver << 1, range_left, range_mid, req_left, min(req_right, range_mid))
            + get(segment_tree, (ver << 1) + 1, range_mid+1, range_right, max(req_left, range_mid+1), req_right); 
}

void
update(
        Vertex * segment_tree, 
        int32_t ver, 
        int32_t range_left, 
        int32_t range_right, 
        int32_t req_left, 
        int32_t req_right, 
        int32_t delta)
{
    if (req_left > req_right) {
        return;
    }
    if (req_left == range_left && req_right == range_right) {
        segment_tree[ver].value += delta;
        return;
    }
    int32_t range_mid = (range_left + range_right) >> 1;
    segment_tree[ver << 1].value += segment_tree[ver].value;
    segment_tree[(ver << 1) + 1].value += segment_tree[ver].value;
    segment_tree[ver].value = 0;
    update(segment_tree, ver << 1, range_left, range_mid, req_left, min(req_right, range_mid), delta);
    update(segment_tree, (ver << 1) + 1, range_mid + 1, range_right, max(req_left, range_mid+1), req_right, delta);
    segment_tree[ver].pre_sum = segment_tree[ver << 1].value * (range_mid - range_left + 1) 
            + segment_tree[ver << 1].pre_sum
            + segment_tree[(ver << 1) + 1].value * (range_right - range_mid) + segment_tree[(ver << 1) + 1].pre_sum;
}

int
main(void)
{
    int32_t n, number_commands;
    if (scanf("%d %d", &n, &number_commands) != 2) {
        fprintf(stderr, "Input error!\n");
        return 1;
    }
    Vertex * segment_tree = calloc(4 * n, sizeof(*segment_tree));
    if (segment_tree == NULL) {
        fprintf(stderr, "Memory allocation error!\n");
        return 2;
    }
    for (size_t i = 0; i < number_commands; i++) {
        int32_t command, req_left, req_right, delta;
        if (scanf("%d %d %d", &command, &req_left, &req_right) != 3) {
            fprintf(stderr, "Input error!\n");
            return 1;
        }
        req_right--;
        if (command == 1) {
            if (scanf("%d", &delta) != 1) {
                fprintf(stderr, "Input error!\n");
                return 1;
            }
            update(segment_tree, 1, 0, n - 1, req_left, req_right, delta);
        } else if (command == 2) {
            printf("%d\n", get(segment_tree, 1, 0, n-1, req_left, req_right));
        } else {
            fprintf(stderr, "Unknown command\n");
            return 3;
        }
    }
    free(segment_tree);
    return 0;
}
