//
// Created by Vladislav Dankevich on 20.10.2024.
// parent process pid > 0, child process pid == 0, pid < 0 - error
//
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {
    pid_t pid = fork();  // Fork a child process

    if (pid == 0) {
        // Child process
        printf("Child process (PID: %d) is terminating.\n", getpid());
        exit(0);  // Child terminates
    }
    if (pid > 0) {
        // Parent process
        printf("Parent process (PID: %d).\n", getpid());
        sleep(5);
        waitpid(pid, NULL, 0);
        printf("Reaped child process (PID: %d).\n", pid);
    } else {
        perror("fork failed");
    }

    return 0;
}
