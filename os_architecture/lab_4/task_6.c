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

void create_fifo() {
    // by default access - 0666
    if (mkfifo(FIFO_PATH, 0666) == -1) {
        perror("mkfifo");
        exit(EXIT_FAILURE);
    }
    printf("FIFO created: %s\n", FIFO_PATH);
}

void writer() {
    int fd;
    char buffer[BUFFER_SIZE];

    // open FIFO for write only
    fd = open(FIFO_PATH, O_WRONLY);
    if (fd == -1) {
        perror("open for writing");
        exit(EXIT_FAILURE);
    }

    printf("Enter messages to send. Type 'exit' to quit.\n");

    while (1) {
        printf("Message: ");

        fgets(buffer, BUFFER_SIZE, stdin);

        if (strstr(buffer, "exit") == 0) {
            break;
        }

        // writing to fifo
        if (write(fd, buffer, strlen(buffer) + 1) == -1) {
            perror("write");
            close(fd);
            exit(EXIT_FAILURE);
        }
    }
}

void reader() {
    int fd;
    char buffer[BUFFER_SIZE];

    // open FIFO for read only
    fd = open(FIFO_PATH, O_RDONLY);
    if (fd == -1) {
        perror("open for reading");
        exit(EXIT_FAILURE);
    }
    printf("Waiting for messages...\n");

    while (1) {
        ssize_t bytes_read = read(fd, buffer, BUFFER_SIZE);
        if (bytes_read > 0) {
            printf("Received: %s\n", buffer);
        } else if (bytes_read == -1) {
            perror("read");
            close(fd);
            exit(EXIT_FAILURE);
        }
        sleep(1); // create delay to check O_NDELAY behavior (it's async file opening)
        // https://www.opennet.ru/man.shtml?topic=open&category=2
    }
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <mode>\nModes: create, write, read\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    if (strcmp(argv[1], "create") == 0) {
        create_fifo();
    } else if (strcmp(argv[1], "write") == 0) {
        writer();
    } else if (strcmp(argv[1], "read") == 0) {
        reader();
    } else {
        fprintf(stderr, "Unknown mode: %s\n", argv[1]);
        exit(EXIT_FAILURE);
    }

    return 0;
}
