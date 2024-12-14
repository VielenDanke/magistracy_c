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
        sleep(5);
        int fd = open(filename, O_RDONLY);
        if (fd == -1) {
            perror("open for reading");
            exit(1);
        }

        char buffer[sizeof(message)];

        while (ssize_t bytes_read = read(fd, buffer, sizeof(buffer)) > 0) {
            if (bytes_read > 0) {
                write(STDOUT_FILENO, buffer, bytes_read);
            } else if (bytes_read == 0) {
                printf("EOF\n");
                close(fd);
                exit(0);
            } else {
                perror("read");
                close(fd);
                exit(1);
            }
            sleep(1);
        }
    }
    int fd = open(filename, O_WRONLY);

    if (fd == -1) {
        perror("open for writing");
        exit(1);
    }
    int i = 0;

    while (i++ < 1000) {
        ssize_t written_bytes = write(fd, message, strlen(message));
        if (written_bytes == -1) {
            perror("write");
            close(fd);
            exit(1);
        }
        if (written_bytes == 0) {
            printf("Empty message. Stop\n");
            break;
        }
        sleep(1);
    }
    close(fd);

    return 0;
}
