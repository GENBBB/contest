#include <stdio.h>
void normalize_path(char *buf);
int 
main (int argc, char *argv[])
{
    normalize_path(argv[1]);
    printf("%s\n", argv[1]);
    return 0;
}
