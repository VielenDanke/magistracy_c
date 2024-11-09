#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
//
// Created by Vladislav Dankevich on 27.10.2024.
// No iteration without timeout
//
//
int main() {
    pid_t pid = fork();

    if (pid < 0) {
        perror("Fork failed ");
        exit(1);
    }
    if (pid == 0) {
        printf("child process started %d\n", getpid());
        for (int i = 0; i < 10; i++) {
            for (int j = 0; j < 10000; j++) {
                for (int p = 0; p < 100000; p++) {

                }
            }
            printf("%d\n", i);
        }
    }
    printf("parent process started %d. child process %d\n", getpid(), pid);
    sleep(2);
    if (kill(pid, SIGUSR1) == -1) { // #define SIGUSR1 30      /* user defined signal 1 */
        perror("kill failed ");
        exit(1);
    }
    int status;
    waitpid(pid, &status, 0);
    printf("child process finished with status %d\n", status); // status 30
    // status is equal to what we sent using kill command
}
