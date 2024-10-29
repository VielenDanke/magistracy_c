#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/_types/_pid_t.h>
//
// Created by Vladislav Dankevich on 28.10.2024.
//
int main() {
    pid_t pid = fork();

    if (pid < 0) {
        perror("Fork failed ");
        exit(1);
    }
    if (pid == 0) {
        printf("child process started %d\n", getpid());
        pause();
    }
    printf("parent process started %d. child process %d\n", getpid(), pid);
    sleep(2);
    if (kill(pid, SIGKILL) == -1) {
        perror("kill failed ");
        exit(1);
    }
    int status;
    wait(&status);
    printf("child process finished with status %d\n", status);
}