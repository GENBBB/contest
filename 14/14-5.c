#include <stdio.h>
#include <signal.h>
#include <unistd.h>

volatile sig_atomic_t p_num = 0;
volatile sig_atomic_t flag = 0;

void
handler(int sig)
{
    if (flag == 3) {
        _exit(0);
    }
    printf("%d\n", p_num);
    fflush(stdout);
    flag++;
}

void
handler_exit(int sig)
{
    _exit(0);
}

int
main(int argc, char *argv[])
{
    sigaction(SIGINT, &(struct sigaction) {.sa_handler = handler, .sa_flags = SA_RESTART}, NULL);
    sigaction(SIGTERM, &(struct sigaction) {.sa_handler = handler_exit, .sa_flags = SA_RESTART}, NULL);
    int low, high;
    scanf("%d", &low);
    scanf("%d", &high);
    printf("%d\n", getpid());
    fflush(stdout);
    int p_flag = 0;
    for (int i = low; i < high; i++) {
        if (i < 2) {
            continue;
        }
        p_flag = 1;
        for (int j = 2; j * j <= i; j++) {
            if (i % j == 0) {
                p_flag = 0;
                break;
            }
        }
        if (p_flag) {
            p_num = i;
        }
    }
    printf("-1\n");
    fflush(stdout);
    return 0;
}