//
// Created by Vladislav Dankevich on 20.10.2024.
//
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>

// if both are writing to the file the parent process is dominating ?
// read operation is available only in parent process?
int main(int argc, char *argv[]) {
    pid_t pid;
    int fd, status;
    char buf[256];

    fd = open(argv[1], O_RDONLY);

    pid = fork();

    if (pid == 0) {
        printf("child %d %d %d\n", getpid(), getppid(), getpgid(getpid()));
        int fd_child = creat("/Users/vladislavdankevich/CLionProjects/untitled/os_architecture/lab_2/text_child_task_8.txt", 0777);
        int cn = read(fd, buf, sizeof(buf));
        printf("child read %s\n", buf);
        write(fd_child, buf, cn);
        close(fd_child);
        fd_child = open("/Users/vladislavdankevich/CLionProjects/untitled/os_architecture/lab_2/text_child_task_8.txt", O_RDONLY);
        read(fd_child, buf, sizeof(buf));
        printf("text_child.txt: %s\n", buf);
        exit(0);
    }
    if (pid > 0) {
        printf("parent %d %d %d\n", getpid(), getppid(), getpgid(getpid()));
        int fd_parent = creat("/Users/vladislavdankevich/CLionProjects/untitled/os_architecture/lab_2/text_parent_task_8.txt", 0777);
        int pn = read(fd, buf, sizeof(buf));
        printf("parent read %s\n", buf);
        write(fd_parent, buf, pn);
        close(fd_parent);
        wait(&status);
        fd_parent = open("/Users/vladislavdankevich/CLionProjects/untitled/os_architecture/lab_2/text_parent_task_8.txt", O_RDONLY);
        read(fd_parent, buf, sizeof(buf));
        printf("text_parent.txt: %s\n", buf);
    } else {
        perror("fork failed");
        exit(1);
    }
    //выход из родительского процесса
    close(fd);

    return 0;
}
