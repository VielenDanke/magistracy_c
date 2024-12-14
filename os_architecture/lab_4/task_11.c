//
// Created by Vladislav Dankevich on 14.12.2024.
//
//
// Created by Vladislav Dankevich on 14.12.2024.
//
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/fcntl.h>
#include <sys/wait.h>

int main() {
    pid_t pid = fork();
    if (pid == -1) {
        perror("fork");
        exit(1);
    }

    if (pid == 0) {
        struct flock lock;

        lock.l_type = F_RDLCK; // Тип блокировки: чтение (shared)
        lock.l_whence = SEEK_SET; // Откуда начинать блокировку: от начала файла
        lock.l_start = 0; // Начальное смещение: от начала
        lock.l_len = 0; // Длина блокировки: весь файл

        if (fcntl(STDIN_FILENO, F_SETLK, &lock) == -1) {
            perror("fcntl (setlk)");
            exit(1);
        }

        char buffer[1024];

        while (1) {
            fgets(buffer, sizeof(buffer), stdin);
            printf("Дочерний процесс: %s", buffer);
            fflush(stdout); // Очищаем буфер вывода
        }
        lock.l_type = F_UNLCK;

        if (fcntl(STDIN_FILENO, F_SETLK, &lock) == -1) {
            perror("fcntl (unlock)");
            exit(1);
        }
    }
    struct flock lock;

    lock.l_type = F_RDLCK; // Тип блокировки: чтение (shared)
    lock.l_whence = SEEK_SET; // Откуда начинать блокировку: от начала файла
    lock.l_start = 0; // Начальное смещение: от начала
    lock.l_len = 0; // Длина блокировки: весь файл

    if (fcntl(STDIN_FILENO, F_SETLK, &lock) == -1) {
        perror("fcntl (setlk)");
        exit(1);
    }
    // Родительский процесс
    char buffer[1024];

    while (1) {
        fgets(buffer, sizeof(buffer), stdin);
        printf("Родительский процесс: %s", buffer);
        fflush(stdout); // Очищаем буфер вывода
    }
    lock.l_type = F_UNLCK;

    if (fcntl(STDIN_FILENO, F_SETLK, &lock) == -1) {
        perror("fcntl (unlock)");
        exit(1);
    }
    wait(NULL); // Ожидание завершения дочернего процесса

    return 0;
}
