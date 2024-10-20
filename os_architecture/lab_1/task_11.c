//
// Created by Vladislav Dankevich on 20.10.2024.
//

#include <stdio.h>
#include <sys/stat.h>

int main(int argc, char *argv[]) {
    if (argc == 1) {
        perror("No files provided");
        return 1;
    }
    int max = 0, ret;
    struct stat file_stat;
    char *f_name;

    for (int i = 0; i < argc - 1; i++) {
        if ((ret = stat(argv[i + 1], &file_stat)) != 0) {
            printf("stat failure error .%d", ret);
        } else {
            if (max < file_stat.st_size) {
                max = file_stat.st_size;
                f_name = argv[i];
            }
        }
    }
    printf("%s: %d", f_name, max);

    return 0;
}
