#include <stdio.h>
typedef struct Point
{
    double x;
    double y;
} Point;

int
main(void)
{
    const Point LEFT_DOWN = {2.0, 1.0};
    const Point RIGHT_UP = {5.0, 7.0};
    const Point RIGHT_DOWN = {5.0, 3.0};
    const Point CENTER_DOWN = {3.0, 1.0};
    Point verify;
    if (scanf("%lf", &verify.x) != 1) {
        fprintf(stderr, "Input error!\n");
        return 1;
    }
    if (scanf("%lf", &verify.y) != 1) {
        fprintf(stderr, "Input error!\n");
        return 1;
    }
    if (verify.y <= RIGHT_UP.y && verify.x >= LEFT_DOWN.x && verify.x <= RIGHT_DOWN.x && verify.y >= LEFT_DOWN.y) {
        if (verify.x - CENTER_DOWN.x <= verify.y - CENTER_DOWN.y) {
            printf("1\n");
            return 0;
        } else {
            printf("0\n");
            return 0;
        }
    }
    printf("0\n");
    return 0;
}
