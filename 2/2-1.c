#include<stdio.h>
int
main(void)
{
    char s1[129];
    char s2[129];
    char s3[129];
    if (scanf("%s", s1) != 1) {
        return 1;
    }
    if (scanf("%s", s2) != 1) {
        return 1;
    }
    if (scanf("%s", s3) != 1) {
        return 1;
    }
    printf("[Host:%s,Login:%s,Password:%s]\n", s1, s2, s3);
    return 0;
}
