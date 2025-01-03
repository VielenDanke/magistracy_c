//
// Created by Vladislav Dankevich on 19.10.2024.
//  gcc task_1.c -o task_1
// ./task_1 1>info.log
// ./task_1 2>err.log
//  ./task_1 task_1.c - correct
//  ./task_1 task_1 - incorrect
//

#include <stdio.h>

#include <unistd.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>

int check_fd(const int fd) {
    if (fd == -1) {
        perror("Fail");
        printf("File descriptor = %d\n", errno);
        printf("From strerror: %s\n", strerror(errno));
        return 1;
    }

    return 0;
}

int main(int argc, char *argv[]) {
    // const int fd = open(argv[1], O_RDONLY);
    // if (check_fd(fd) != 1) {
    //     printf("Open successfully: %s. File descriptor = %d\n", argv[1], fd);
    // } else {
    //     printf("From sys_errlist: %s. Index in the list: %d\n", sys_errlist[errno], errno);
    // }
    // return 0;
    printf("%d\n", 2304 / 256); // SIGKILL
}
