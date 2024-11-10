//
// Created by Vladislav Dankevich on 20.10.2024.
//
/*
 * child process appears in `ps` after we stopped the program using CTRL-C
 * CTRL-C won't affect child process
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/_types/_pid_t.h>

// Повлияет ли на порожденный процесс нажатие клавиши прерывания в тот момент, когда родительский процесс "спит"? Нет
// Будет ли показан порожденный процесс в выводе команды ps? Да
/*
setpgrp() создает новую группу процессов и делает текущий процесс лидером этой группы.  Это важно для управления процессами, особенно при обработке сигналов.

Влияние нажатия Ctrl+C:

Без setpgrp(): Если бы setpgrp() не было, то при нажатии Ctrl+C (SIGINT) и родительский, и дочерний процессы получили бы этот сигнал и завершились бы.
С setpgrp(): Так как дочерний процесс теперь в своей собственной группе процессов, сигнал SIGINT, отправленный при нажатии Ctrl+C, будет доставлен только родительскому процессу (и процессам в его группе). Дочерний процесс продолжит "спать" (выполнять pause()).
Вывод команды ps:

Да, дочерний процесс будет показан в выводе команды ps.  Вы можете использовать ps -o pid,ppid,pgrp,comm чтобы увидеть PID, PPID (PID родительского процесса), PGRP (ID группы процессов) и имя команды. Вы заметите, что PGRP дочернего процесса отличается от PGRP родительского процесса, что подтверждает, что он находится в своей собственной группе.
 */
int main() {
    pid_t pid = fork();  // Fork a child process

    pid_t current_process_pid = getpid();

    if (pid == 0) {
        // Child process
        pid_t spgrp = setpgrp(); // creates a group of processes and nominate current process as a leader
        if (spgrp < 0) {
            perror("setgrp");
            exit(1);
        }
        printf("Child process %d. Group %d\n", current_process_pid, spgrp);
        pause();
        printf("Child process %d finished\n", current_process_pid);
        exit(0);  // Child terminates
    }
    if (pid > 0) {
        // Parent process
        printf("Parent process %d\n", current_process_pid);
        pause();
        printf("Parent process %d finished\n", current_process_pid);
        exit(0);
    }
    perror("fork failed");

    return 0;
}
