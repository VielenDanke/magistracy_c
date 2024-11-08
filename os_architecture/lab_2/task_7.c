//
// Created by Vladislav Dankevich on 20.10.2024.
//
/*
 * child process appears in `ps` after we stopped the program using CTRL-C
 * CTRL-C won't affect child process
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/_types/_pid_t.h>

// Повлияет ли на порожденный процесс нажатие клавиши прерывания в тот момент, когда родительский процесс "спит"? Нет
// Будет ли показан порожденный процесс в выводе команды ps? Да
int main() {
    pid_t pid = fork();  // Fork a child process

    pid_t current_process_pid = getpid();

    if (pid == 0) {
        // Child process
        pid_t spgrp = setpgrp(); // creates a group of processes and nominate current process as a leader
        if (spgrp < 0) {
            perror("setgrp");
            exit(1);
        }
        printf("Child process %d. Group %d\n", current_process_pid, spgrp);
        pause();
        printf("Child process %d finished\n", current_process_pid);
        exit(0);  // Child terminates
    }
    if (pid > 0) {
        // Parent process
        printf("Parent process %d\n", current_process_pid);
        pause();
        printf("Parent process %d finished\n", current_process_pid);
        exit(0);
    }
    perror("fork failed");

    return 0;
}
