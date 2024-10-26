//
// Created by Vladislav Dankevich on 26.10.2024.
//

#include <stdio.h>
#include <signal.h>
#include <unistd.h>

void signal_handler(int signum) {
    printf("Receive signal %d\n", signum);
    signal(signum, SIG_DFL);
}

int main() {
    struct sigaction new_action;

    new_action.sa_handler = signal_handler;

    sigaction(SIGINT, &new_action, NULL);
    printf("Program is ready. Press Ctrl+C...\n");

    while (1) {
        sleep(1);
    }
}