//
// Created by Vladislav Dankevich on 20.10.2024.
//

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// if parent process died the parent become init process (PID 1) - start when OS loads
int main() {
    pid_t pid;

    pid = fork();

    if (pid < 0) {
        perror("fork failed");
        exit(1); // exit parent process
    }
    if (pid == 0) {
        printf("child %d %d %d\n", getpid(), getppid(), getpgid(getpid()));
        sleep(3); // wait 3 seconds to allow parent process to finish first
        printf("child process finished\n");
        exit(0);
    }
    printf("parent %d %d %d\n", getpid(), getppid(), getpgid(getpid()));
    printf("parent process finished\n");
    exit(0);
}
