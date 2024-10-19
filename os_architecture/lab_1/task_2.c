//
// Created by Vladislav Dankevich on 19.10.2024.
//

/*
./task_2 new_file.txt 0444 - not possible to write because of permission mode
./task_2 new_file.txt 0644 - owner read, write, other only read
./task_2 new_file.txt 0666 - all can read and write

For the permission 0644, here's how it breaks down:

0: Special mode (not commonly used in basic permissions)
6: rw- (Owner: Read and Write, but no execute)
4: r-- (Group: Read only)
4: r-- (Others: Read only)

Octal	Permission String	Description
0755	rwxr-xr-x	Owner can read, write, and execute; group and others can read and execute.
0644	rw-r--r--	Owner can read and write; group and others can only read.
0777	rwxrwxrwx	Everyone can read, write, and execute (full access for all).
0700	rwx------	Owner can read, write, and execute; no access for group and others.
0600	rw-------	Owner can read and write; no access for group and others.
0555	r-xr-xr-x	Owner, group, and others can read and execute, but no one can write.
0444	r--r--r--	Owner, group, and others can only read, no one can write or execute.
0222	--w--w--w-	Owner, group, and others can only write.
0111	--x--x--x	Owner, group, and others can only execute.
*/

#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
    int fd;
    const char buf[] = "Text to write to a file\n";

    unsigned int access_mode = strtol(argv[2], NULL, 8);

    if ((fd = creat(argv[1], access_mode)) != -1) {
        int n;
        printf("Create successfully: %s\n", argv[1]);

        if ((n = write(fd, buf, sizeof(buf) - 1)) != -1) {
            printf("Let's open and read %s:\n", argv[1]);

            if ((fd = open(argv[1], O_RDONLY)) != -1) {
                char b[512];
                int nbyte;
                if ((nbyte = read(fd, b, sizeof b)) != 0) {
                    int wbytes;
                    if ((wbytes = write(1, b, nbyte)) != nbyte) {
                        printf("Write error: %s\n", strerror(errno));
                    } else {
                        printf("Written %d bytes\n", wbytes);
                    }
                } else {
                    printf("Read failed: %s\n", strerror(errno));
                }
                if (close(fd) != -1) {
                    printf("Let's open %s with r+w mode:\n", argv[1]);
                    if ((fd = open(argv[1], O_RDWR)) != -1) {
                        write(1, b, nbyte);
                    } else {
                        printf("Open failed: (%d) %s fd %d.\n", errno, strerror(errno), fd);
                    }
                } else {
                    printf("Close failed: %s\n", strerror(errno));
                }
            } else {
                printf("Open failed: %s\n", strerror(errno));
            }
        } else {
            printf("%d Write failed: %s\n", n, strerror(errno));
        }
    } else {
        perror("Creation failed");
    }
    return 0;
}
