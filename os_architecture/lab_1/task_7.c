//
// Created by Vladislav Dankevich on 20.10.2024.
//

#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char *argv[]) {
    int ret;
    struct stat file_stat;

    if ((ret = stat(argv[1], &file_stat)) != 0) {
        printf("stat failure error .%d", ret);
        perror("error");
        exit(1);
    }
    printf("File permissions: %o\n", file_stat.st_mode & 07777); // remove first 100 from the number 100644
    printf("Number of hard links:%d\n", file_stat.st_nlink);
    printf("System block size in bytes: %d\n", file_stat.st_blksize);
    printf("File size in bytes: %lld\n", file_stat.st_size);
    printf("File size in blocks: %lld\n", file_stat.st_blocks);
    printf("User ID:%d\n", file_stat.st_uid);
    printf("Group ID:%d\n", file_stat.st_gid);
    printf("Last access time:\t\t\t%s", asctime(localtime(&file_stat.st_atime)));
    printf("Last content modification time:\t\t%s", asctime(localtime(&file_stat.st_mtime)));
    printf("Last attributes modification time:\t%s", asctime(localtime(&file_stat.st_ctime)));

    return 0;
}
