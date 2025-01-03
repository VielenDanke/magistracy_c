#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/_types/_pid_t.h>
//
// Created by Vladislav Dankevich on 28.10.2024.
//
/*
Child’s State	    Suspended, waiting for a signal	                        Actively running nested loops
Termination	        Triggered by SIGKILL (forced termination)	            Normally completes loops or terminated by SIGKILL
Parent’s wait()	    Waits until the child receives a termination signal	    Waits until the child exits, either naturally or by signal
*/
int main() {
    pid_t pid = fork();

    if (pid < 0) {
        perror("Fork failed ");
        exit(1);
    }
    if (pid == 0) {
        printf("child process started %d\n", getpid());
        while (1) {
            pause();
            printf("iterate \n");
        }
    }
    printf("parent process started %d. child process %d\n", getpid(), pid);
    sleep(2);
    if (kill(pid, SIGINT) == -1) {
        perror("kill failed ");
        exit(1);
    }
    int status;
    wait(&status);
    printf("child process finished with status %d\n", status);
}
