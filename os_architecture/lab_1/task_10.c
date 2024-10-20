#include <stdio.h>
#include <unistd.h>
#include <sys/fcntl.h>
//
// Created by Vladislav Dankevich on 20.10.2024.
//
int main(int argc, char *argv[]) {
    int fd = open(argv[1], O_RDONLY);
    char buf;

    if (fd > 0) {
        int n;
        if ((n = lseek(fd, 0, SEEK_END)) > 0) {
            for (int i = 1; i < n; i++) {
                lseek(fd, -i, SEEK_END);
                if (read(fd, &buf, 1) > 0) {
                    printf("%c", buf);
                } else {
                    perror("Read failed");
                }
            }
        } else {
            perror("File is empty");
        }
    } else {
        perror("Can't open file");
    }
}
