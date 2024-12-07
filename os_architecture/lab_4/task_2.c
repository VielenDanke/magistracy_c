//
// Created by Vladislav Dankevich on 06.12.2024.
//
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

int main() {
    int pipefd[2];  // Create a channel

    // Check if creating the channel is failed
    if (pipe(pipefd) == -1) {
        perror("pipe");
        exit(1);
    }
    pid_t pid = fork();

    if (pid < 0) {
        perror("fork");
        exit(1);
    }
    if (pid == 0) {
        printf("Child process\n");
        // Close read from a channel
        close(pipefd[0]);

        // Read from stdin and write to a channel
        char buffer[1024];

        while (fgets(buffer, sizeof(buffer), stdin) != NULL) {
            write(pipefd[1], buffer, strlen(buffer) + 1);
        }
        // close channel, signal of EOF. Parent process is receiving 0 as a signal of EOF
        close(pipefd[1]);
    } else {
        printf("Parent process\n");
        // Close writing to a channel
        close(pipefd[1]);

        // Reading while the channel is not empty and there is no EOF signal
        char buffer[1024];
        int bytes_read;
        while ((bytes_read = read(pipefd[0], buffer, sizeof(buffer))) > 0) {
            printf("Parent process received: %s", buffer);
        }
        if (bytes_read == -1) {
            perror("read ");
        }
        // Close reading descriptor
        close(pipefd[0]);

        // Waiting for a child process to finish
        wait(&pid);
    }
    return 0;
}