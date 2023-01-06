#include <stdio.h>
#include <unistd.h>
#include <signal.h>

volatile sig_atomic_t flag = 1;

void
handler_add(int sig)
{
    flag = 1;
}

void
handler_mull(int sig)
{
    flag = 0;
}

int
main(void)
{
    sigaction(SIGINT, &(struct sigaction) {.sa_handler = handler_add, .sa_flags = SA_RESTART}, NULL);
    sigaction(SIGQUIT, &(struct sigaction) {.sa_handler = handler_mull, .sa_flags = SA_RESTART}, NULL);
    int result = 0;
    int num;
    printf("%d\n", getpid());
    fflush(stdout);
    while (scanf("%d", &num) == 1) {
        if (flag) {
            __builtin_sadd_overflow(num, result, &result);
        } else {
            __builtin_smul_overflow(num, result, &result);

        }
        printf("%d\n", result);
        fflush(stdout);
    }
    return 0;
}