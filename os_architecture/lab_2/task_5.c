//
// Created by Vladislav Dankevich on 20.10.2024.
// parent process pid > 0, child process pid == 0, pid < 0 - error
//
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

// can we kill zombie process because it's already dead?
int main() {
    pid_t pid = fork();  // Fork a child process

    if (pid == 0) {
        // Child process
        printf("Child process (PID: %d) is terminating.\n", getpid());
        // pid_t parent_pid = getppid();
        // kill(parent_pid, SIGCHLD);
        exit(0);  // Child terminates
    }
    if (pid > 0) {
        // Parent process
        printf("Parent process (PID: %d).\n", getpid());
        waitpid(pid, NULL, 0); // wait for child process to finish
        printf("Reaped child process (PID: %d).\n", pid);
    } else {
        perror("fork failed");
    }

    return 0;
}
