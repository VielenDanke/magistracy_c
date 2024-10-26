//
// Created by Vladislav Dankevich on 20.10.2024.
// first to execute - parent, second - child
//
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
    pid_t pid, cur_pid;
    char *s;

    pid = fork();
    printf("current pid %d\n", pid);

    if (pid == -1) {
        perror("fork failed");
        exit(1); //выход из родительского процесса
    }
    if (pid == 0) {
        cur_pid = getpid();
        printf("child with pid %d\n", cur_pid);
        exit(0);
    }
    cur_pid = getpid();
    printf("PARENT with pid %d\n", cur_pid);
    exit(0);
}
