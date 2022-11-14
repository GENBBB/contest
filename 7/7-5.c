#include <time.h>
#include <stdio.h>

enum
{
    FIRST_YEAR = 1900,
    FM_DAY = 26,
    FM_MONTH = 4,
    FM_YEAR = 2021,
    FM_HOUR = 11,
    FM_MINUTE = 14,
    FM_PER_DAY = 29,
    FM_PER_HOUR = 12,
    FM_PER_MINUTE = 44,
    BORDER_KEY = 256,
    DATA_SIZE = 100
};

void
tm_init(struct tm *time, int year, int month,int day, int hour, int min, int sec, int dst) {
    time->tm_year = year - FIRST_YEAR;
    time->tm_mon = month;
    time->tm_mday = day;
    time->tm_hour = hour;
    time->tm_min = min;
    time->tm_sec = sec;
    time->tm_isdst = dst;
    return;
}

int
main (int argc, char *argv[])
{
    int year;
    char result[DATA_SIZE];
    if (scanf("%d", &year) != 1) {
        return 1;
    }
    year -= FIRST_YEAR;
    struct tm f_m;
    tm_init(&f_m, FM_YEAR, FM_MONTH, FM_DAY, FM_HOUR, FM_MINUTE, 0, -1);
    time_t full_moon = timegm(&f_m);
    struct tm f_m_per;
    tm_init(&f_m_per, 1970, 0, FM_PER_DAY + 1, FM_PER_HOUR, FM_PER_MINUTE, 0, -1);
    time_t full_moon_per = timegm(&f_m_per);
    if (year < FM_YEAR - FIRST_YEAR) {
        while (f_m.tm_year != year) {
            full_moon -= full_moon_per;
            gmtime_r(&full_moon, &f_m);
        }
        while (f_m.tm_yday >= BORDER_KEY) {
            full_moon -= full_moon_per;
            gmtime_r(&full_moon, &f_m);
        }
        full_moon += full_moon_per;
        gmtime_r(&full_moon, &f_m);
        f_m.tm_mday += (f_m.tm_wday == 1) ? 7 : (8 - f_m.tm_wday) % 7;
        f_m.tm_mday += 21;
        timegm(&f_m);
        strftime(result, DATA_SIZE, "%Y-%m-%d", &f_m);
        printf("%s\n", result);
    } else {
        while (f_m.tm_year != year) {
            full_moon += full_moon_per;
            gmtime_r(&full_moon, &f_m);
        }
        while (f_m.tm_yday <= BORDER_KEY) {
            full_moon += full_moon_per;
            gmtime_r(&full_moon, &f_m);
        }
        f_m.tm_mday += (f_m.tm_wday == 1) ? 7 : (8 - f_m.tm_wday) % 7;
        f_m.tm_mday += 21;
        timegm(&f_m);
        strftime(result, DATA_SIZE, "%Y-%m-%d", &f_m);
        printf("%s\n", result);
    }
    return 0;
}
