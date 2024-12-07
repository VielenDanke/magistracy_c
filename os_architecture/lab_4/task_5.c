//
// Created by Vladislav Dankevich on 07.12.2024.
//
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    int pipefd[2]; // Создаем канал

    // Проверяем, успешно ли создан канал
    if (pipe(pipefd) == -1) {
        perror("pipe");
        exit(1);
    }

    pid_t pid1 = fork();

    if (pid1 < 0) {
        perror("fork");
        exit(1);
    }
    if (pid1 == 0) {
        printf("Child process with WHO\n");

        close(pipefd[0]);

        dup2(pipefd[1], STDOUT_FILENO);
        close(pipefd[1]);

        execlp("who", "who", NULL);

        perror("execlp ");
        exit(1);
    }
    printf("Parent process\n");

    pid_t pid2 = fork();

    if (pid2 < 0) {
        perror("fork");
        exit(1);
    }
    if (pid2 == 0) {
        printf("Child process with WC\n");

        close(pipefd[1]);

        dup2(pipefd[0], STDIN_FILENO);
        close(pipefd[0]);

        execlp("wc", "wc", "-l", NULL);

        perror("execlp ");
        exit(1);
    }
    close(pipefd[0]);
    close(pipefd[1]);

    wait(&pid2);
    wait(&pid1);

    return 0;
}