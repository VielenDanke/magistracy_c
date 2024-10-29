//
// Created by Vladislav Dankevich on 26.10.2024.
//

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
    pid_t pid = fork();

    if (pid < 0) {
        perror("Fork failed ");
        exit(1);
    }
    if (pid == 0) {
        for (int i = 0; i < 100; i++) {
            for (int j = 0; j < 1000; j++) {
                for (int p = 0; p < 10000; p++) {

                }
            }
            printf("%d\n", i);
        }
        exit(9);
    }
    int status;
    wait(&status);
    printf("child process finished with status %d\n", status);
}
