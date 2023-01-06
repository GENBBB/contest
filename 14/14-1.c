#include <stdio.h>
#include <unistd.h>
#include <signal.h>

volatile sig_atomic_t flag = -1;

void
handler(int sig)
{
    if (flag == 4) {
        _exit(0);
    }
    flag++;
    printf("%d\n", flag);
    fflush(stdout);
}

int
main(void)
{
    sigaction(SIGHUP, &(struct sigaction) {.sa_handler = handler, .sa_flags = SA_RESTART}, NULL);
    printf("%d\n", getpid());
    fflush(stdout);
    while (1) {
        pause();
    }
    return 0;
}