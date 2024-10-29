//
// Created by Vladislav Dankevich on 26.10.2024.
//

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>


void action(int signal) {
    int status, p;
    sleep(1);

    while ((p = wait(&status)) != -1) {
        // the same as waitpid(-1, &status, 0); waiting process to finish
        if (WIFEXITED(status)) { // child process finished normally if it's not equal to 0
            printf("Signal %d. Exit status of the child %d was %d\n", signal, p, WEXITSTATUS(status));
        }
    };
}

int main() {
    signal(SIGCHLD, action); // SIGCHLD - to parent on child stop or exit. Install action on SIGCHLD command

    for (int i = 0; i < 10; i++) {
        if (fork() == 0) {
            printf("child %d\n", getpid());
            sleep(5);
            exit(1);
        }
    }

    pause(); // pause execution
}
