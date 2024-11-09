//
// Created by Vladislav Dankevich on 20.10.2024.
//
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>

int main(int argc, char *argv[], char *envp[]) {
    pid_t pid;
    const char *f = argv[1];
    argv++;

    pid = fork();
    if (pid < 0) {
        perror("fork failed");
        exit(1); //выход из родительского процесса
    }
    if (pid == 0) {
        printf("child process\n");
        execvp(f, argv); //execvp (file, argv)
        printf("child process finished\n"); // group of exec() functions replaces the current process image therefore the printf won't be executed
        exit(0);
    }
    printf("parent process\n");
    system("ls -l");
    printf("parent process finished\n");
    exit(0);
}
