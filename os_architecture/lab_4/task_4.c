//
// Created by Vladislav Dankevich on 07.12.2024.
//
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

/*
 * Дескрипторы каналов передаются в порожденный процесс автоматически при вызове fork.
 * fork создает копию процесса, включая все открытые файловые дескрипторы.
 * Поэтому дочерний процесс наследует дескрипторы канала от родительского процесса.
 */
int main() {
    // declare a unidirectional channel
    int pipefd[2];

    // create the channel
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

        // close read descriptor
        close(pipefd[0]);

        // redirect STDOUT descriptor (1) to write channel descriptor
        dup2(pipefd[1], STDOUT_FILENO);

        // close original write descriptor
        close(pipefd[1]);

        // executing ls command to print out to STDOUT all the files in the directory
        execlp("ls", "ls", "-l", NULL);

        // if execlp returns to the current process meaning error occurred
        perror("execlp ");
        exit(1);
    }
    printf("Parent process\n");

    // close write descriptor
    close(pipefd[1]);

    // reading from a channel
    char buffer[1024];
    ssize_t bytes_read;
    while ((bytes_read = read(pipefd[0], buffer, sizeof(buffer))) > 0) {
        printf("Parent process received: %s", buffer);
    }

    // if bytes_read returns -1 declare error
    if (bytes_read == -1) {
        perror("read ");
    }

    // close read descriptor
    close(pipefd[0]);

    // waiting for a child process
    wait(&pid);

    return 0;
}
