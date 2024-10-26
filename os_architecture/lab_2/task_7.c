//
// Created by Vladislav Dankevich on 20.10.2024.
//
/*
 * child process appears in `ps` after we stopped the program using CTRL-C
 * CTRL-C won't affect child process
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/_types/_pid_t.h>

int main() {
    pid_t pid = fork();  // Fork a child process

    if (pid == 0) {
        // Child process
        pid_t spgrp = setpgrp(); // creates a group of processes and nominate current process as a leader
        if (spgrp < 0) {
            perror("setgrp");
            exit(1);
        }
        printf("Child %d %d %d %d\n", getpid(), getppid(), getpgid(getpid()), spgrp);
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
