//
// Created by Vladislav Dankevich on 14.12.2024.
//
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/file.h> // для flock

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
        exit(1);
    }
    char message[] = "Hello from writer!\n";

    const char *filename = argv[1];

    pid_t pid = fork();
    if (pid == -1) {
        perror("fork");
        exit(1);
    }


    if (pid == 0) {
        sleep(1);

        int fd = open(filename, O_RDONLY);
        if (fd == -1) {
            perror("open for reading");
            exit(1);
        }
        struct flock lock;

        lock.l_type = F_RDLCK; // Тип блокировки: чтение (shared)
        lock.l_whence = SEEK_SET; // Откуда начинать блокировку: от начала файла
        lock.l_start = 0; // Начальное смещение: от начала
        lock.l_len = 0; // Длина блокировки: весь файл

        if (fcntl(fd, F_SETLK, &lock) == -1) {
            perror("fcntl (setlk)");
            exit(1);
        }

        char buffer[sizeof(message)];

        while (1) {
            ssize_t bytes_read = read(fd, buffer, sizeof(buffer));
            if (bytes_read > 0) {
                write(STDOUT_FILENO, buffer, bytes_read);
            } else if (bytes_read == 0) {
                printf("EOF\n");
                break;
            } else {
                perror("read");
                break;
            }
        }
        lock.l_type = F_UNLCK;
        if (fcntl(fd, F_SETLK, &lock) == -1) {
            perror("fcntl (unlock)");
            exit(1);
        }
        close(fd);
    }
    int fd = open(filename, O_WRONLY, 0777);

    if (fd == -1) {
        perror("open for writing");
        exit(1);
    }
    struct flock lock;

    lock.l_type = F_RDLCK;
    lock.l_whence = SEEK_SET;
    lock.l_start = 0;
    lock.l_len = 0;

    if (fcntl(fd, F_SETLK, &lock) == -1) {
        perror("fcntl (setlk)");
        exit(1);
    }

    int i = 0;

    while (i++ < 1000) {
        ssize_t written_bytes = write(fd, message, strlen(message));
        if (written_bytes == -1) {
            perror("write");
            break;
        }
        if (written_bytes == 0) {
            printf("Empty message. Stop\n");
            break;
        }
    }
    lock.l_type = F_UNLCK;

    if (fcntl(fd, F_SETLK, &lock) == -1) {
        perror("fcntl (unlock)");
        exit(1);
    }
    close(fd);

    return 0;
}
