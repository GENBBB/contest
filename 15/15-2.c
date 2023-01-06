#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/signalfd.h>
#include <limits.h>

void
recv_proc(int sfd)
{
    char buf = 0;
    struct signalfd_siginfo msg;
    while (1) {
        for (int i = 0; i < sizeof(buf) * CHAR_BIT; i++) {
            if (read(sfd, &msg, sizeof(msg)) != sizeof(msg)) {
                close(sfd);
                _exit(1);
            }
            if (msg.ssi_signo == SIGUSR2) {
                buf = buf | (1 << i);
            } else if (msg.ssi_signo == SIGIO) {
                close(sfd);
                fflush(stdout);
                _exit(0);
            }
            kill(msg.ssi_pid, SIGALRM);
        }
        putchar(buf);
        buf = 0;
    }
}

void
send_proc(char *file, pid_t recv, int sfd)
{
    int fd = open(file, O_RDONLY);
    char buf;
    if (fd == -1) {
        kill(recv, SIGIO);
        _exit(1);
    }
    struct signalfd_siginfo alarm;
    while (read(fd, &buf, sizeof(buf)) == sizeof(buf)) {
        for (int i = 0; i < sizeof(buf) * CHAR_BIT; i++) {
            if (buf & 1) {
                kill(recv, SIGUSR2);
            } else {
                kill(recv, SIGUSR1);
            }
            if (read(sfd, &alarm, sizeof(alarm)) != sizeof(alarm)) {
                close(sfd);
                kill(recv, SIGIO);
                _exit(1);
            }
            buf = buf >> 1;
        }
    }
    kill(recv, SIGIO);
    close(fd);
    close(sfd);
    _exit(0);
}

int
main(int argc, char *argv[])
{
    sigset_t set;
    sigemptyset(&set);
    sigaddset(&set, SIGUSR2);
    sigaddset(&set, SIGUSR1);
    sigaddset(&set, SIGIO);
    sigaddset(&set, SIGALRM);
    sigprocmask(SIG_BLOCK, &set, NULL);
    int sfd_send = signalfd(-1, &set, 0);
    sigemptyset(&set);
    sigaddset(&set, SIGALRM);
    int sfd_alarm = signalfd(-1, &set, 0);
    pid_t recv = fork();
    if (!recv) {
        close(sfd_alarm);
        recv_proc(sfd_send);
    } else if (recv < 0) {
        _exit(1);
    }
    close(sfd_send);
    pid_t send = fork();
    if (!send) {
        send_proc(argv[1], recv,sfd_alarm);
        _exit(0);
    } else if (send < 0) {
        _exit(1);
    }
    close(sfd_alarm);
    wait(NULL);
    wait(NULL);
    return 0;

}