//
// Created by Vladislav Dankevich on 16.11.2024.
//
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

volatile sig_atomic_t signal_count = 0;

void signal_handler(int signum) {
    sigset_t mask, oldmask;

    // Заполняем маску всеми сигналами
    sigfillset(&mask);

    // Блокируем все сигналы
    if (sigprocmask(SIG_BLOCK, &mask, &oldmask) < 0) {
        perror("sigprocmask");
        exit(1);
    }

    // Имитируем длительную обработку сигнала
    printf("Start signal %d for process %d\n", signum, getpid());
    sleep(5);
    signal_count++;
    printf("Finish signal %d for process %d\n", signum, getpid());

    // Восстанавливаем предыдущую маску сигналов
    if (sigprocmask(SIG_SETMASK, &oldmask, NULL) < 0) {
        perror("sigprocmask");
        exit(1);
    }
}

int main() {
    struct sigaction sa;

    // Настраиваем обработчик сигнала
    sa.sa_handler = signal_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;

    // Устанавливаем обработчик для SIGINT и SIGUSR1
    if (sigaction(SIGUSR2, &sa, NULL) < 0 || sigaction(SIGUSR1, &sa, NULL) < 0) {
        perror("sigaction");
        exit(1);
    }

    pid_t pid = fork();

    if (pid < 0) {
        perror("fork failed ");
        exit(1);
    }
    if (pid == 0) {
        printf("child process with pid %d\n", getpid());
        sleep(5);
        exit(0);
    }
    printf("PID процесса: %d\n", getpid());
    printf("Ожидание сигналов...\n");

    sleep(2);

    if (kill(pid, SIGUSR1) == -1) {
        perror("kill failed ");
        exit(1);
    }
    sleep(1);
    if (kill(pid, SIGUSR2) == -1) {
        perror("kill failed ");
        exit(1);
    }
    sleep(15);

    return 0;
}