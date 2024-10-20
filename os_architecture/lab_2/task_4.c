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
    switch (pid) {
        case -1:
            perror("fork failed");
            exit(1); //выход из родительского процесса
        case 0:
            printf("child\t%d\t%d\t%d\n", getpid(), getppid(), getpgid(getpid()));
            sleep(3);
            printf("child process finished\n");
            exit(0);
        default:
            printf("PARENT\t%d\t%d\t%d\n", getpid(), getppid(), getpgid(getpid()));
            printf("PARENT process finished\n");
            exit(0);
    }
}
