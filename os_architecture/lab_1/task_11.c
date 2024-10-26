//
// Created by Vladislav Dankevich on 20.10.2024.
//

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

int main(int argc, char *argv[]) {
    if (argc == 1) {
        perror("No files provided");
        return 1;
    }
    int max = -1;
    struct stat file_stat;
    char *f_name = NULL;

    for (int i = 1; i < argc; i++) {
        const int ret = stat(argv[i], &file_stat);

        if (ret == 0) {
            if (max <= file_stat.st_size) {
                max = file_stat.st_size;
                f_name = argv[i];
            }
        }
    }
    if (f_name == NULL) {
        perror("No files provided");
    }
    printf("%s: %d", f_name, max);

    return 0;
}
