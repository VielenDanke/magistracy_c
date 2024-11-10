//
// Created by Vladislav Dankevich on 20.10.2024.
//
/*
 * `ps` to check all the processes in the system
 * `kill -9` to kill process
 * the child process still in the `ps` because now it's attached to parent process (PID 1)?
 * CTRL-C kills both parent and child processes
 * We cannot do the same after parent process is finished, because parent process ends up the programm
 * and child process become attached to a parent process
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/_types/_pid_t.h>

// if 2 processes are on pause, the processes are waiting for a termination signal
// if only child process is on pause, when parent process is finished, the child process will be attached to the main process PID 1 (when OS loads)
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
