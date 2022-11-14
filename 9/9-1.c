#include <stdlib.h>
#include <unistd.h>

int proc(void)
{
    int pid = fork();
    if (!pid) {
        int n = write(1, "1\n", 2);
        n++;
    }
    return pid;
}

int
main(void) {
    proc(), proc(), proc();
    return 0;
}