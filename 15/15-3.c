#include <stdio.h>
#include <unistd.h>
#include <sys/sem.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>

int
main(int argc, char *argv[])
{
    enum {
        ACCESS_RIGHTS = 0666
    };
    key_t key = ftok(argv[0], 0);
    int n;
    sscanf(argv[1], "%d", &n);
    int sem_id = semget(key, n, IPC_CREAT | ACCESS_RIGHTS);
    struct sembuf up;
    up.sem_flg = 0;
    up.sem_op = 1;
    struct sembuf down;
    down.sem_flg = 0;
    down.sem_op = -1;
    setbuf(stdin, NULL);
    for (int i = 0; i < n; i++) {
        pid_t pid = fork();
        if (!pid) {
            down.sem_num = i;
            while (1) {
                if (semop(sem_id, &down, 1) == -1) {
                    exit(0);
                }
                int num;
                if (scanf("%d", &num) != 1) {
                    exit(0);
                }
                fflush(stdin);
                printf("%d %d\n", i, num);
                fflush(stdout);
                up.sem_num = (n + num % n) % n;
                semop(sem_id, &up, 1);
            }
        } else if (pid == -1) {
            semctl(sem_id, 0, IPC_RMID);
            while (wait(NULL) > 0) {}
            return 1;
        }
    }
    up.sem_num = 0;
    semop(sem_id, &up, 1);
    wait(NULL);
    semctl(sem_id, 0, IPC_RMID);
    while (wait(NULL) > 0) {}
    return 0;
}