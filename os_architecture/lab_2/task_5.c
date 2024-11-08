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
// kill -s SIGCHLD <PID родительского процесса>
// kill <PID родительского процесса>
int main() {
    pid_t pid = fork(); // Fork a child process

    if (pid < 0) {
        perror("fork failed");
        exit(1);
    }
    if (pid == 0) {
        // Child process
        sleep(3);
        printf("child process finished\n");
        exit(0);
    }
    // Parent process
    sleep(10);
    printf("parent process finished\n");
    return 0;
}
