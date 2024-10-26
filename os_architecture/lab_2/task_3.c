//
// Created by Vladislav Dankevich on 20.10.2024.
//
/*
If fork() returns a negative value, the creation of a child process was unsuccessful.
fork() returns a zero to the newly created child process.
fork() returns a positive value, the process ID of the child process, to the parent. The returned process ID is of type pid_t defined in sys/types.h. Normally, the process ID is an integer. Moreover, a process can use function getpid() to retrieve the process ID assigned to this process.
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {
    pid_t pid;
    int status, ch_pid;

    pid = fork();

    if (pid < 0) {
        perror("fork failed");
        exit(1); //выход из родительского процесса
    }
    if (pid == 0) {
        printf("child %d %d %d\n", getpid(), getppid(), getpgid(getpid()));
        exit(0);
    }
    printf("parent %d %d %d\n", getpid(), getppid(), getpgid(getpid()));
    ch_pid = wait(&status); // wait, waitpid - wait for a child process to stop or terminate
    printf("parent: child process %d finished\n", ch_pid);
    if (WIFEXITED(status)) { // returns true if a child process terminates normally
        printf("child exited with code %d\n", WIFEXITED(status));
    } else {
        printf("child terminated abnormally\n");
    }
    exit(0);
}
