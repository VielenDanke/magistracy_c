//
// Created by Vladislav Dankevich on 19.10.2024.
//
/*
 * gcc task_4.c -o task_4
 * ./task_4 new_file.txt
* 1. SEEK_SET
Description: Set the file offset to a specific value.
Behavior: The offset is set relative to the beginning of the file. The new file offset is exactly the value provided.
Example: lseek(fd, 10, SEEK_SET) — Moves the file pointer to the 10th byte from the beginning of the file.
2. SEEK_CUR
Description: Set the file offset relative to the current position of the file pointer.
Behavior: The provided offset is added (or subtracted, if negative) to the current file pointer position.
Example: lseek(fd, 5, SEEK_CUR) — Moves the file pointer 5 bytes forward from its current position. lseek(fd, -5, SEEK_CUR) moves it 5 bytes backward.
3. SEEK_END
Description: Set the file offset relative to the end of the file.
Behavior: The provided offset is added to the file size. Typically used to append data or read from the end of the file.
Example: lseek(fd, 0, SEEK_END) — Moves the file pointer to the end of the file. lseek(fd, -10, SEEK_END) moves the file pointer 10 bytes before the end of the file.
4. SEEK_DATA (Linux-specific, available from kernel version 3.1+)
Description: Move the file pointer to the next location in the file where there is actual data (i.e., not a "hole").
Behavior: Useful for sparse files where parts of the file may not physically exist on disk (e.g., in filesystems that support sparse files).
Example: lseek(fd, 0, SEEK_DATA) — Moves the file pointer to the first byte of data after the specified offset.
5. SEEK_HOLE (Linux-specific, available from kernel version 3.1+)
Description: Move the file pointer to the next "hole" in the file (a region that doesn’t contain data, often found in sparse files).
Behavior: Used to locate empty regions within a file, particularly in sparse files.
Example: lseek(fd, 0, SEEK_HOLE) — Moves the file pointer to the first hole after the specified offset.
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

    if ((fd = creat(argv[1], 0600)) != -1) {
        int n;
        if ((n = write(fd, buf, sizeof(buf) - 1)) != -1) {
            const int n1 = n / 2;

            if ((fd = open(argv[1], O_RDWR)) != -1) {
                char b[512];
                int rbytes;
                if ((rbytes = read(fd, b, n)) != 0) {
                    printf("read bytes: %d\n", rbytes);
                    write(1, b, n);
                    // set char b[512] all values to 0
                    memset(b, 0, sizeof(b));

                    // read from the position n1 number of bytes n1 + 1 without changing file pointer
                    rbytes = pread(fd, b, n1 + 1, n1); //(,,count,offset)
                    printf("pointer in the middle: %s. Read bytes %d\n", b, rbytes);
                    memset(b, 0, sizeof(b));

                    // 0 bytes before the file end - no results were read
                    lseek(fd, 0, SEEK_END);
                    rbytes = read(fd, b, n1);
                    printf("SEEK_END+0: %s. Read bytes: %d\n", b, rbytes);
                    memset(b, 0, sizeof(b));

                    // move pointer on 10 positions before file beginning - results in error
                    int pos = lseek(fd, -10, SEEK_SET);
                    rbytes = read(fd, b, n1);
                    printf("SEEK_SET-10: %s. Position: %d. Read bytes: %d\n", b, pos, rbytes);
                    if (errno) {
                        perror("error ");
                    }
                } else {
                    perror("Read failed");
                }
            } else {
                perror("Open failed");
            }
        } else {
            perror("Write failed");
        }
    }

    return 0;
}
