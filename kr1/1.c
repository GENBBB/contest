#include <time.h>
#include <stdio.h>

int free_day(struct tm *cur)
{
    if (cur->tm_wday == 0 || cur->tm_wday == 6) {
        return 1;
    }
    int bin = 1;
    for (int i = 0; i < 9; i++) {
        if (cur->tm_yday == bin) {
            return 1;
        }
        bin <<= 1;
    }
    return 0;
}

int
main(int argc, char *argv[])
{
    int year;
    if (scanf("%d", &year) != 1) {
        return 1;
    }
    struct tm data;
    data.tm_year = year - 1900;
    data.tm_isdst = -1;
    int result = 0;
    for (int i = 0; i < 12; i++) {
        for (int j = 1; j < 32; j++) {
            data.tm_mon = i;
            data.tm_mday = j;
            data.tm_hour = 12;
            data.tm_min = 2;
            data.tm_sec = 3;
            time_t cur_t = mktime(&data);
            struct tm result_data;
            gmtime_r(&cur_t, &result_data);
            if (result_data.tm_mon != i) {
                continue;
            }
            if (free_day(&result_data)) {
                result++;
            }
        }
    }
    printf("%d\n", result);
    return 0;
}
