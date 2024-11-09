//
// Created by Vladislav Dankevich on 26.10.2024.
//

#include <stdio.h>
#include <signal.h>
#include <unistd.h>

struct sigaction old_sig_action;

void signal_handler(int signum) {
    printf("Receive signal %d\n", signum);
    sigaction(signum, &old_sig_action, NULL);
}

int main() {
    struct sigaction new_action;

    new_action.sa_handler = signal_handler;

    sigaction(SIGINT, &new_action, &old_sig_action);

    printf("Program is ready. Press Ctrl+C to end the execution and send a signal to a custom handler\n");

    while (1) {
        sleep(1);
    }
}