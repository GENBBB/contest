#include <stdio.h>
#include <unistd.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <limits.h>

int
main(int argc, char *argv[])
{
    enum {
        HEX = 16,
        KEY_ARG = 2,
        MAX_ARG = 3,
        ACCESS_RIGHTS = 0660,
        SIZE_SHM = 2
    };
    key_t key = strtol(argv[KEY_ARG], NULL, HEX);
    int n,max;
    sscanf(argv[1], "%d", &n);
    sscanf(argv[MAX_ARG], "%d", &max);
    int sem_id = semget(key, n, IPC_CREAT | ACCESS_RIGHTS);
    int shm_id = shmget(key, SIZE_SHM * sizeof(int), IPC_CREAT | ACCESS_RIGHTS);
    int *shm_addr = shmat(shm_id, NULL, 0);
    struct sembuf up;
    up.sem_flg = 0;
    up.sem_op = 1;
    struct sembuf down;
    down.sem_flg = 0;
    down.sem_op = -1;
    for (int i = 0; i < n; i++) {
        pid_t pid = fork();
        if (!pid) {
            down.sem_num = i;
            while (1) {
                if (semop(sem_id, &down, 1) == -1) {
                    exit(0);
                }
                printf("%d %d %d\n", i + 1, shm_addr[0], shm_addr[1]);
                fflush(stdout);
                if (*shm_addr == INT_MAX || ++(*shm_addr) > max) {
                    exit(0);
                }
                shm_addr[1] = i + 1;
                up.sem_num = ((*shm_addr % n) * (*shm_addr % n) % n) * ((*shm_addr % n) * (*shm_addr % n) % n) % n;
                semop(sem_id, &up, 1);
            }
        } else if (pid == -1) {
            shmctl(shm_id, IPC_RMID, 0);
            semctl(sem_id, 0, IPC_RMID);
            while (wait(NULL) > 0) {}
            return 1;
        }
    }
    shm_addr[0] = 0;
    shm_addr[1] = 0;
    up.sem_num = 0;
    semop(sem_id, &up, 1);
    wait(NULL);
    shmctl(shm_id, IPC_RMID, 0);
    semctl(sem_id, 0, IPC_RMID);
    while (wait(NULL) > 0) {}
    return 0;
}