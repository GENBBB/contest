#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

enum
{
    N_COMMAND = 2,
    N_GET = 2,
    N_UPT = 1
};

int32_t 
get(int32_t *arr, int32_t req_left, int32_t req_right)
{
    int32_t result = 0;
    for (size_t i = req_left; i < req_right; i++) {
        result += arr[i];
    }
    return result;
}

void
update(int32_t *arr, int32_t req_left, int32_t req_right, int32_t add)
{
    for (size_t i = req_left; i < req_right; i++) {
        arr[i] += add;
    }
}

int
main(void)
{
    int32_t n, number_commands;
    int32_t *arr;
    if (scanf("%d%d", &n, &number_commands) != N_COMMAND) {
        fprintf(stderr, "Input error!\n");
        return 1;
    }
    arr = calloc(n, sizeof(*arr));
    if (arr == NULL) {
        fprintf(stderr, "Memory allocation error!\n");
        return 1;
    }
    for (size_t i = 0; i < number_commands; i++) {
        int32_t command, req_left, req_right, add;
        if (scanf("%d%d%d", &command, &req_left, &req_right) != 3) {
            fprintf(stderr, "Input error!\n");
            return 1;
        }
        if (command == N_UPT) {
            if (scanf("%d", &add) != 1) {
                fprintf(stderr, "Input error!\n");
                return 1;
            }
            update(arr, req_left, req_right, add);
        } else if (command == N_GET) {
            printf("%d\n", get(arr, req_left, req_right));
        } else {
            fprintf(stderr, "Unknown command\n");
            return 1;
        }
    }
    free(arr);
    return 0;
}
