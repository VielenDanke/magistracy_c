//
// Created by Vladislav Dankevich on 20.10.2024.
//

#include <stdio.h>
#include <_stdlib.h>

extern char **environ; // holds the environment variables of the process

int main(int argc, char *argv[], char *envp[]) {
    int i = 0;
    char *var = "Test Variable";
    char *env_key = "TEST_VAR";

    if (setenv(env_key, var, 0) != 0) {
        perror("setenv failed");
        exit(1);
    }
    printf("%s\n", getenv(env_key));

    unsetenv(env_key);

    for (char **env = environ; *env != NULL; env++) {
        printf("%s\n", *env);
    }
    return 0;
}
