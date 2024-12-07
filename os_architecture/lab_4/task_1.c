#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
//
// Created by Vladislav Dankevich on 06.12.2024.
//
int main() {
    int pipefd_uni[2]; // Unidirectional
    int pipefd_bi[2];  // Bidirectional

    // Create channels
    if (pipe(pipefd_uni) == -1 || pipe(pipefd_bi) == -1) {
        perror("pipe error ");
        exit(1);
    }

    pid_t pid = fork();

    if (pid < 0) {
        perror("fork failed ");
        exit(1);
    }
    if (pid == 0) {
        printf("child process\n");
        // Close unnecessary descriptors to avoid deadlocks
        // The descriptors must be mutually exclusive in child-parent processes
        close(pipefd_uni[1]); // Close write of uni-directional channel ddescriptor
        close(pipefd_bi[0]); // Close read of bidirectional channel descriptor

        // Read from uni-directional channel
        char buf_uni[100];
        read(pipefd_uni[0], buf_uni, sizeof(buf_uni));
        printf("Child process: received from one-direction channel: %s\n", buf_uni);

        // Write to bidirectional channel
        char *msg_bi = "Child process say hi";
        write(pipefd_bi[1], msg_bi, strlen(msg_bi) + 1);
        printf("Child process: Send to bidirectional channel\n");

        // Read from bidirectional channel
        read(pipefd_bi[1], buf_uni, sizeof(buf_uni));
        printf("Child process: received from bidirectional channel: %s\n", buf_uni);

        close(pipefd_uni[0]);
        close(pipefd_bi[1]);

        exit(0);
    }
    // Close unnecessary descriptors to avoid deadlocks
    // The descriptors must be mutually exclusive in child-parent processes
    close(pipefd_uni[0]); // Close read of uni-directional channel descriptor
    close(pipefd_bi[1]); // Close write of bidirectional channel ddescriptor

    // Write to uni-directional
    char *msg_uni = "Parent process say hi";
    write(pipefd_uni[1], msg_uni, strlen(msg_uni) + 1);
    printf("Parent process: send to uni-directional channel\n");

    // Read from bidirectional
    char buf_bi[100];
    read(pipefd_bi[0], buf_bi, sizeof(buf_bi));
    printf("Parent process: received from bidirectional channel: %s\n", buf_bi);

    // Write to bidirectional
    char *msg_bi2 = "Parent process say hi second time";
    write(pipefd_bi[0], msg_bi2, strlen(msg_bi2) + 1);
    printf("Parent process: send to bidirectional channel\n");

    close(pipefd_uni[1]);
    close(pipefd_bi[0]);

    // Ожидаем завершения дочернего процесса
    wait(&pid);
}