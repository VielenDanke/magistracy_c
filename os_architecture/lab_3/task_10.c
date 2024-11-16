#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
//
// Created by Vladislav Dankevich on 29.10.2024.
//
// Custom signal handler for SIGUSR1
void sigusr1_handler(int sig) {
    printf("Signal %d. Child process (PID %d) received SIGUSR1!\n", sig, getpid());
    sleep(2);
    printf("End of signal %d for PID %d\n", sig, getpid());
}

void sigusr2_handler(int sig) {
    printf("Signal %d. Child process (PID %d) received SIGUSR2!\n", sig, getpid());
    sleep(2);
    printf("End of signal %d for PID %d\n", sig, getpid());
}

int main() {
    pid_t pid = fork();

    if (pid == -1) {
        perror("fork failed");
        exit(1);
    }
    if (pid == 0) { // Child process
        // Install the signal handler for SIGUSR1
        struct sigaction su1, su2;
        su1.sa_handler = sigusr1_handler;
        sigemptyset(&su1.sa_mask);
        su1.sa_flags = 0;
        if (sigaction(SIGUSR1, &su1, NULL) == -1) {
            perror("sigaction failed");
            exit(1);
        }
        // Install the signal handler for SIGUSR2
        su2.sa_handler = sigusr2_handler;
        sigemptyset(&su2.sa_mask);
        su2.sa_flags = 0;
        if (sigaction(SIGUSR2, &su2, NULL) == -1) {
            perror("sigaction failed");
            exit(1);
        }
        printf("Child process (PID %d) waiting for signal...\n", getpid());
        while (1) {
            sleep(1);
        }
    }
    // Parent process
    printf("Parent process (PID %d) sending signal to child (PID %d)...\n", getpid(), pid);
    sleep(2);
    if (kill(pid, SIGUSR1) == -1) {
        perror("kill failed");
        exit(1);
    }
    sleep(1);
    if (kill(pid, SIGUSR2) == -1) {
        perror("kill failed");
        exit(1);
    }
    sleep(5); // Wait a bit before sending the signal
    printf("Parent process sent signal.\n");

    return 0;
}
