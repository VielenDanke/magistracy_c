//
// Created by Vladislav Dankevich on 19.10.2024.
// https://ru.wikipedia.org/wiki/Разрежённый_файл
//
/*
 */

#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

int main(int argc, char *argv[]) {
    const char buf[] = "0Lorem_0ipsum_2Lorem_2ipsum_3Lorem_3ipsum";
    int fd;
    struct stat file_stat;

    if ((fd = open(argv[1], O_WRONLY | O_CREAT | O_TRUNC, 0600)) != -1) {
        int n;
        if ((n = write(fd, buf, sizeof(buf) - 1)) != -1) {
            int n1 = n / 2;

            if ((fd = open(argv[1], O_RDWR)) != -1) {
                char b[512];
                int rbytes;
                if ((rbytes = read(fd, b, n)) > 0) {
                    int ret;
                    printf("Read bytes: %d\n", rbytes);
                    write(1, b, n);
                    memset(b, 0, sizeof(b));

                    rbytes = pread(fd, b, n1 + 1, n1); //(,,count,offset)
                    printf("Pointer in the middle: %s. Bytes: %d\n", b, rbytes);
                    memset(b, 0, sizeof(b));

                    lseek(fd, -20, SEEK_END);
                    rbytes = read(fd, b, n1);
                    printf("SEEK_END+0: %s. Bytes: %d\n", b, rbytes);

                    lseek(fd, 5, SEEK_SET);
                    // perror("error "); will print invalid argument if lseek with SEEK_SET -1
                    if ((rbytes = read(fd, b, n1)) >= 0) {
                        printf("SEEK_SET-10: %s. Bytes: %d\n", b, rbytes);
                    } else {
                        perror("error ");
                    }
                    //sparse file
                    memset(b, 0, sizeof(b));
                    const int f_size = lseek(fd, 0,SEEK_END);
                    printf("File size in bytes: %d\n", f_size);
                    lseek(fd, 8 * 1024,SEEK_SET);
                    write(fd, buf, sizeof(buf) - 1);

                    if ((ret = stat(argv[1], &file_stat)) != 0) {
                        printf("stat failure error .%d", ret);
                    } else {
                        printf("Sparse file size in bytes: %lld\n", file_stat.st_size);
                        printf("System block size in bytes: %d\n", file_stat.st_blksize);
                        printf("Sparse file size in blocks: %lld\n", file_stat.st_blocks);
                    }
                } else {
                    perror("Read failed");
                }
            } else {
                perror("Open failed");
            }
        } else
            perror("Write failed");
    }

    return 0;
}
