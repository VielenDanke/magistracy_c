//
// Created by Vladislav Dankevich on 20.10.2024.
//
/*
 * Steps:
 * 1. Modify current program to handle SIGINT command (CTRL-C) differently
 * 2. After handling SIGINT command restore default behavior
 * 3. Press CTRL-C to shut down the process
 */

#include <stdio.h>
#include <signal.h>
#include <unistd.h>

void signal_handler(int signum) {
    printf("Receive signal %d\n", signum);
    signal(signum, SIG_DFL);
}

int main() {
    signal(SIGINT, signal_handler);

    printf("Program is ready. Press Ctrl+C to end the execution and send a signal to a custom handler\n");

    while (1) {
        sleep(1);
    }
}
