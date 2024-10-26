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
        write(fd, buf, sizeof(buf) - 1);
        //sparse file
        int ret;
        const int f_size = lseek(fd, 0, SEEK_END);
        printf("File size in bytes: %d\n", f_size);
        lseek(fd, 1024 * 1024 * 1024, SEEK_SET);
        write(fd, buf, sizeof(buf) - 1);

        if ((ret = stat(argv[1], &file_stat)) != 0) {
            printf("stat failure error .%d", ret);
        } else {
            printf("Sparse file size in bytes: %lld\n", file_stat.st_size);
            printf("System block size in bytes: %d\n", file_stat.st_blksize);
            printf("Sparse file size in blocks: %lld\n", file_stat.st_blocks);
        }
    }

    return 0;
}
