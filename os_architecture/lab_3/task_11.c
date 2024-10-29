#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/signal.h>
#include <sys/mman.h>
#include <signal.h>

#define s1 "00\n"
#define s2 "11\n"

static sigset_t omask2;

void handler1(int signo) {
    write(1, s1, sizeof(s1)); // 1 - base output
}

void handler2(int signo) {
    write(1, s2, sizeof(s2)); // 1 - base output
}

int main(void) {
    pid_t pid, p_pid;
    static struct sigaction action, ch_action;
    sigset_t mask;
    int c = 0;
    sigfillset(&mask);

    action.sa_handler = handler1; // provide signal handler
    action.sa_flags = 0; // default behavior fir signal handler
    sigemptyset(&action.sa_mask); // make all signals non-blocked
    sigaction(SIGUSR1, &action, NULL);

    ch_action.sa_handler = handler2;
    sigaction(SIGUSR2, &ch_action, NULL);
    ch_action.sa_flags = 0; // default behavior fir signal handler
    sigemptyset(&ch_action.sa_mask); // make all signals non-blocked
    sigprocmask(SIG_BLOCK, &mask,NULL);

    pid = fork();

    if (pid < 0) {
        perror("Fork failed");
        exit(1);
    }
    if (pid == 0) {
        p_pid = getppid();

        sigdelset(&mask, SIGUSR2);

        while (c != 5) {
            c++;
            sleep(1);
            kill(p_pid, SIGUSR1);
            sigsuspend(&mask);
        }

        exit(0);
    }
    sigdelset(&mask, SIGUSR1);

    while (c != 5) {
        c++;
        sleep(1);
        sigsuspend(&mask);
        kill(pid, SIGUSR2);
    }

    wait(NULL);
    sleep(2);
    exit(0);
}
