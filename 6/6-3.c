#include<time.h>
#include<stdio.h>
int
main(void) {
    int year;
    scanf("%d", &year);
    year = year - 1900;
    struct tm data;
    data.tm_year = year;
    data.tm_isdst = -1;
    for (int i = 0; i < 12; i++) {
        int number_wed = 0;
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
            if (result_data.tm_wday == 4) {
                number_wed++;
            }
            if (result_data.tm_mday %3 == 0) {
                continue;
            }
            if (result_data.tm_wday == 4 && (number_wed == 2 || number_wed == 4)) {
                printf("%d %d\n", i+1, result_data.tm_mday);
            }
        }
    }
    return 0;
}
