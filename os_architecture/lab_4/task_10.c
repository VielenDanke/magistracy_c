//
// Created by Vladislav Dankevich on 14.12.2024.
//
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    pid_t pid = fork();
    if (pid == -1) {
        perror("fork");
        exit(1);
    }

    if (pid == 0) {
        // Дочерний процесс
        char buffer[1024];
        while (1) {
            fgets(buffer, sizeof(buffer), stdin);
            printf("Дочерний процесс: %s", buffer);
            fflush(stdout); // Очищаем буфер вывода
        }
    }
    // Родительский процесс
    char buffer[1024];
    while (1) {
        fgets(buffer, sizeof(buffer), stdin);
        printf("Родительский процесс: %s", buffer);
        fflush(stdout); // Очищаем буфер вывода
    }
    wait(NULL); // Ожидание завершения дочернего процесса

    return 0;
}