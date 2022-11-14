#include <stdio.h>
#include <stdlib.h>
#include <math.h>

enum
{
    ROUND_ACCURACY = 10000,
    PERCENT_SCALE = 100,
    N_FIRST_PERCENT = 2
};

double
change_rate(double rate, double percent)
{
    return ((PERCENT_SCALE + percent) / PERCENT_SCALE) * rate;
}

int 
main(int argc, char *argv[])
{
    if (argc == 1) {
        return 1;
    }
    double current_rate = strtod(argv[1], NULL);
    current_rate = round(current_rate * ROUND_ACCURACY) / ROUND_ACCURACY;
    for (int i = N_FIRST_PERCENT; i < argc; i++) {
        double percent = strtod(argv[i], NULL);
        percent = round(percent * ROUND_ACCURACY) / ROUND_ACCURACY;
        current_rate = change_rate(current_rate, percent);
        current_rate = round(current_rate * ROUND_ACCURACY) / ROUND_ACCURACY;
    }
    printf("%.4f\n", current_rate);
    return 0;
}
