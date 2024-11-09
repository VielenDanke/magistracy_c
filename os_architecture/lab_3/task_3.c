//
// Created by Vladislav Dankevich on 26.10.2024.
//

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>

/*
Когда родительский процесс получает SIGCHLD, он может вызвать wait или waitpid в обработчике сигнала. Это позволяет родителю получить код завершения дочернего процесса и освободить связанные с ним ресурсы.
Если SIGCHLD не обрабатывается, завершившийся дочерний процесс останется в состоянии "зомби", пока родительский процесс не вызовет wait или сам не завершится.
*/
void action(int signal) {
    // -1 означает, что нужно ждать любого дочернего процесса, идентификатор группы процессов которого равен абсолютному значению pid.
    // WNOHANG - означает немедленное возвращение управления, если ни один дочерний процесс не завершил выполнение.
    while (waitpid(-1, NULL, WNOHANG) > 0);
}

int main() {
    signal(SIGCHLD, action); // SIGCHLD - to parent on child stop or exit. Install action on SIGCHLD command
    /*
    struct sigaction sa;
    sa.sa_handler = action;
    sa.sa_flags = SA_RESTART; // If the flag is set, returning from a handler resumes the library function.
    sigemptyset(&sa.sa_mask); // Очищаем маску сигналов, которые должны блокироваться при обработке сигнала

    if (sigaction(SIGCHLD, &sa, NULL) == -1) {
        perror("sigaction");
        exit(1);
    }
    */

    // Создание дочернего процесса
    pid_t pid = fork();

    if (pid == -1) {
        perror("fork");
        exit(1);
    }
    pid_t current_process = getpid();

    if (pid == 0) {
        printf("child process %d\n", current_process);
        sleep(2); // Эмуляция работы дочернего процесса
        exit(0); // Завершение дочернего процесса
    }
    printf("parent process %d\n", current_process);
    sleep(5); // Эмуляция работы родителя

    return 0;
}
