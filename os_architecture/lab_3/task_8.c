#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
//
// Created by Vladislav Dankevich on 27.10.2024.
// No iteration without timeout
//
//
void action(int signal) {
    printf("Received signal %d\n", signal);
    sleep(1);
}

int main() {
    pid_t pid = fork();

    if (pid < 0) {
        perror("Fork failed ");
        exit(1);
    }
    if (pid == 0) {
        signal(SIGUSR1, action);
        printf("child process started %d\n", getpid());
        pause();
        exit(0);
    }
    printf("parent process started %d. child process %d\n", getpid(), pid);
    sleep(2);
    if (kill(pid, SIGCHLD) == -1) {
        perror("kill failed ");
        exit(1);
    }
    int status;
    wait(&status);
    printf("child process finished with status %d\n", status);
}
