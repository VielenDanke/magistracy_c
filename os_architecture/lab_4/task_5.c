//
// Created by Vladislav Dankevich on 07.12.2024.
//
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    int pipefd[2];

    if (pipe(pipefd) < 0) {
        perror("pipe");
        exit(1);
    }
    switch (fork()) {
        case -1:
            perror("Fork failed");
            exit(1);
        case 0:
            close(pipefd[0]);
            printf("child process %d\n", getpid());
            dup2(pipefd[1], STDOUT_FILENO);
            execlp("who", "who", NULL);
            exit(EXIT_FAILURE);
        default:
            close(pipefd[1]);
            printf("PARENT process %d\n", getpid());
            dup2(STDOUT_FILENO, pipefd[1]);

            dup2(pipefd[0], STDIN_FILENO);
            execlp("wc", "wc", "-l", NULL);
            exit(EXIT_FAILURE);
    }
}
