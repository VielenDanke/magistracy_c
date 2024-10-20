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
        execvp(f, argv); //execvp (file, argv)
        exit(0);
    }
    system("ls -l");
    exit(0);
}
