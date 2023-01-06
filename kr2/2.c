#include <stdlib.h>
#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/sem.h>
#include <sys/wait.h>

void
handler(int sig)
{
    return;
}

void
process(int sem_id, unsigned l, unsigned p_num, unsigned n)
{
    sigaction(SIGUSR1, &(struct sigaction) {.sa_handler = handler, .sa_flags = SA_RESTART}, NULL);
    struct sembuf up;
    up.sem_flg = 0;
    up.sem_op = 1;
    up.sem_num = 0;
    semop(sem_id, &up, 1);
    while (1) {
        pause();
        l--;
        printf("%d %d\n", p_num, l);
        fflush(stdout);
        up.sem_op = n;
        semop(sem_id, &up, 1);
        if (!l) {
            _exit(0);
        }
    }
}

int
main(int argc, char *argv[])
{
    unsigned n, l;
    sscanf(argv[1], "%u", &n);
    sscanf(argv[2], "%u", &l);
    pid_t *pid = calloc(n, sizeof(*pid));
    key_t key = ftok(argv[0], 0);
    unsigned *live = calloc(n, sizeof(*live));
    int sem_id = semget(key, 1, IPC_CREAT | 0600);
    struct sembuf down;
    down.sem_flg = 0;
    down.sem_op = -n;
    down.sem_num = 0;
    struct sembuf up;
    down.sem_flg = 0;
    down.sem_op = n;
    down.sem_num = 0;
    for (unsigned i = 0; i < n; i++) {
        pid[i] = fork();
        if (!pid[i]) {
            process(sem_id, l, i, n);
        }
    }
    unsigned roulette;
    while (scanf("%u", &roulette) == 1) {
        semop(sem_id, &down, 1);
        if (live[roulette % n] < l) {
            kill(pid[roulette % n], SIGUSR1);
            printf("%d aa\n", roulette);
            fflush(stdout);
            live[roulette % n]++;
        } else {

        }
    }
    unsigned result = 0;
    for (unsigned i = 0; i < n; i++) {
        printf("%d ", live[i]);
        if (live[i] != l) {
            result++;
        }
    }
    putchar('\n');
    printf("%d\n", result);
    fflush(stdout);
    for (unsigned i = 0; i < n; i++) {
        if (live[i] != l) {
            kill(pid[i], SIGKILL);
        }
    }
    while (wait(NULL) > 0) {}
    free(pid);
    free(live);
    semctl(sem_id, 0, IPC_RMID);
    return 0;
}