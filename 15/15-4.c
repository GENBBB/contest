#include<stdio.h>
#include<unistd.h>
#include<sys/msg.h>
#include<sys/wait.h>
#include<stdlib.h>
#include<limits.h>

int main(int argc, char *argv[]) {
    int N;
    long long max;
    key_t key = strtol(argv[1], NULL, 16);
    sscanf(argv[2], "%d", &N);
    sscanf(argv[5], "%lld", &max);
    int id = msgget(key, IPC_CREAT | 0666);
    struct msg {
        long type;
        long long num1;
        long long num2;
    }currmsg;
    currmsg.type = 1;
    sscanf(argv[3], "%lld", &currmsg.num1);
    sscanf(argv[4], "%lld", &currmsg.num2);
    for (int i = 1; i <= N; i++) {
        pid_t pid = fork();
        if (!pid) {
            while (1) {
                if (msgrcv(id, &currmsg,
                        sizeof(currmsg) - sizeof(long), (long)i, 0) == -1) {
                    break;
                }
                long long tmp = currmsg.num2;
                currmsg.num2 += currmsg.num1;
                currmsg.num1 = tmp;
                currmsg.type = currmsg.num2 % N + 1;
                printf("%d %lld\n", i - 1, currmsg.num2);
                fflush(stdout);
                if (currmsg.num2 > max) {
                    break;
                }
                msgsnd(id, &currmsg, sizeof(currmsg) - sizeof(long), 0);
            }
            _exit(0);
        } else if (pid == -1) {
            msgctl(id, IPC_RMID, NULL);
            while (wait(NULL) > 0) {}
            return 1;
        }
    }
    msgsnd(id, &currmsg, sizeof(currmsg) - sizeof(long), 0);
    wait(NULL);
    msgctl(id, IPC_RMID, NULL);
    while (wait(NULL) > 0) {}
    return 0;
}
