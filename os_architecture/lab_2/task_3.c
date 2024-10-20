//
// Created by Vladislav Dankevich on 20.10.2024.
//
/*
If fork() returns a negative value, the creation of a child process was unsuccessful.
fork() returns a zero to the newly created child process.
fork() returns a positive value, the process ID of the child process, to the parent. The returned process ID is of type pid_t defined in sys/types.h. Normally, the process ID is an integer. Moreover, a process can use function getpid() to retrieve the process ID assigned to this process.
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {
    pid_t pid;
    int status, ch_pid;

    pid = fork();

    switch (pid) {
        case -1:
            perror("fork failed");
            exit(1); //выход из родительского процесса
        case 0:
            printf("child\t%d\t%d\t%d\n", getpid(), getppid(), getpgid(getpid()));
            exit(0);
        default:
            printf("PARENT\t%d\t%d\t%d\n", getpid(), getppid(), getpgid(getpid()));
            ch_pid = wait(&status);
            printf("PARENT: child process %d finished\n", ch_pid);
            if (WIFEXITED(status))
                //возвращает ненулевое значение, если порожденный процесс был завершен посредством вызова exit
                printf("child exited with code %d\n", WIFEXITED(status));
            else
                printf("child terminated abnormally\n");
            exit(0);
    }
}
