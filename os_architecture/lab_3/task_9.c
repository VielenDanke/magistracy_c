#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/_types/_pid_t.h>
//
// Created by Vladislav Dankevich on 28.10.2024.
//
void action(int sig){
    printf("signal %d\n", sig);
    sleep(1);
}

int main() {
    pid_t pid = fork();

    if (pid < 0) {
        perror("Fork failed ");
        exit(1);
    }
    if (pid == 0) {
        // signal(SIGALRM, action); turn off or on
        alarm(2);
        alarm(6);
        printf("child process started %d\n", getpid());
        for (int i = 0; i < 10; i++) {
            for (int j = 0; j < 10000; j++) {
                for (int p = 0; p < 100000; p++) {

                }
            }
            printf("%d\n", i);
        }
        exit(0);
    }
    printf("parent process started %d. child process %d\n", getpid(), pid);
    sleep(2);
    int status;
    wait(&status);
    printf("child process finished with status %d\n", status);
}