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

void print_args(char *argv[], char *process, int pid) {
    printf("list of arguments for %s process with pid %d: ", process, pid);
    int i = 0;
    while (argv[i] != NULL) printf("%s ", argv[i++]);
    printf("\n");
}

void print_envs(char *envp[], char *process, int pid) {
    printf("environment for process %s with pid %d: ", process, pid);
    int i = 0;
    while (envp[i] != NULL) printf("%s ", envp[i++]);
    printf("\n");
}

int main(int argc, char *argv[], char *envp[]) {
    pid_t pid;
    char *env[] = {"LANGUAGE=en_US", NULL};

    pid = fork();

    if (pid < 0) {
        perror("fork failed");
        exit(1);
    }
    if (pid == 0) {
        print_args(argv, "child", getpid());
        print_envs(envp, "child", getpid());
        execle("/Users/vladislavdankevich/CLionProjects/untitled/os_architecture/lab_2/task_1", "l1", "l2", "l3", NULL, env);
        exit(0);
    }
    print_args(argv, "parent", getpid());
    print_envs(envp, "parent", getpid());

    return 0;
}
