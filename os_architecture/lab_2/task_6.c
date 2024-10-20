//
// Created by Vladislav Dankevich on 20.10.2024.
//

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/_types/_pid_t.h>

int main() {
    pid_t pid = fork();  // Fork a child process

    if (pid == 0) {
        // Child process
        printf("Child process (PID: %d)\n", getpid());
        pause();
        printf("Child process finished\n");
        exit(0);  // Child terminates
    }
    if (pid > 0) {
        // Parent process
        printf("Parent process (PID: %d).\n", getpid());
        pause();
        printf("Parent process finished\n");
        exit(0);
    }
    perror("fork failed");

    return 0;
}
