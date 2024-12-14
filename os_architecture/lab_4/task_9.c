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
        // Дочерний процесс (читатель)
        int fd = open(filename, O_RDONLY);
        if (fd == -1) {
            perror("open for reading");
            exit(1);
        }

        sleep(3);

        if (flock(fd, LOCK_SH) == -1) {
            perror("flock (shared)");
            exit(1);
        }

        char buffer[sizeof(message)];

        while (1) {
            ssize_t bytes_read = read(fd, buffer, sizeof(buffer));

            if (bytes_read > 0) {
                write(STDOUT_FILENO, buffer, bytes_read);
            } else if (bytes_read == 0) {
                printf("EOF\n");
                close(fd);
                break;
            } else {
                perror("read");
                close(fd);
                break;
            }
        }
        // Снимаем блокировку
        flock(fd, LOCK_UN);
        close(fd);

    } else {
        // Родительский процесс (писатель)
        int fd = open(filename, O_WRONLY, 0644);
        if (fd == -1) {
            perror("open for writing");
            exit(1);
        }
        if (flock(fd, LOCK_SH) == -1) {
            perror("flock (shared)");
            exit(1);
        }

        while (1) {
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
        flock(fd, LOCK_UN);
        close(fd);
    }

    return 0;
}