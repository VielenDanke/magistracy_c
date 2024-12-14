//
// Created by Vladislav Dankevich on 07.12.2024.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

#define FIFO_PATH "/tmp/my_fifo"
#define BUFFER_SIZE 256

int main(int argc, char *argv[]) {
    // by default access - 0666
    if (mkfifo(FIFO_PATH, 0666) == -1) {
        perror("mkfifo");
        exit(EXIT_FAILURE);
    }
    printf("FIFO created: %s\n", FIFO_PATH);

    pid_t pid = fork();

    if (pid == -1) {
        perror("fork failed");
        exit(EXIT_FAILURE);
    }
    if (pid == 0) {
        printf("Child created: %d\n", pid);

        // open FIFO for read only
        int fd = open(FIFO_PATH, O_RDONLY);

        if (fd == -1) {
            perror("open for reading");
            exit(EXIT_FAILURE);
        }
        printf("Waiting for messages...\n");

        while (1) {
            char buffer[BUFFER_SIZE];
            ssize_t bytes_read = read(fd, buffer, BUFFER_SIZE);
            if (bytes_read > 0) {
                printf("Received: %s\n", buffer);
            } else if (bytes_read == -1) {
                perror("read");
                close(fd);
                break;
            }
            sleep(1); // create delay to check O_NDELAY behavior (it's async file opening)
            // https://www.opennet.ru/man.shtml?topic=open&category=2
        }
        printf("Exit-waiting...\n");
    } else {
        printf("Parent created: %d\n", pid);

        // open FIFO for write only
        int fd = open(FIFO_PATH, O_WRONLY);
        if (fd == -1) {
            perror("open for writing");
            exit(EXIT_FAILURE);
        }

        printf("Enter messages to send. Type 'exit' to quit.\n");

        while (1) {
            char buffer[BUFFER_SIZE];
            printf("Type message\n");

            fgets(buffer, BUFFER_SIZE, stdin);

            // writing to fifo
            if (write(fd, buffer, strlen(buffer) + 1) == -1) {
                perror("write");
                close(fd);
                exit(EXIT_FAILURE);
            }
        }
        wait(&pid);
    }
}
