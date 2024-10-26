//
// Created by Vladislav Dankevich on 20.10.2024.
//

#include <stdio.h>
#include <signal.h>
#include <unistd.h>

// Функция-обработчик сигнала
void signal_handler(int signum) {
    printf("Получен сигнал %d\n", signum);

    // Восстанавливаем обработку сигнала по умолчанию
    signal(signum, SIG_DFL);
}

int main() {
    // Устанавливаем обработчик для сигнала SIGINT (Ctrl+C)
    if (signal(SIGINT, signal_handler) == SIG_ERR) {
        perror("Ошибка при вызове signal()");
        return 1;
    }

    printf("Программа запущена. Нажмите Ctrl+C...\n");

    while (1) {
        sleep(1);
    }

    return 0;
}
