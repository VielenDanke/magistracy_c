//
// Created by Vladislav Dankevich on 19.12.2024.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#define MAX_MSG_SIZE 1024

// Структура сообщения
struct message {
    long mtype;
    char mtext[MAX_MSG_SIZE];
};

int main() {
    char *path_to_server_key_file =
            "/Users/vladislavdankevich/CLionProjects/untitled/os_architecture/lab_5/sync_queue";

    key_t key = ftok(path_to_server_key_file, 41);

    if (key == -1) {
        perror("error generating key");
        exit(EXIT_FAILURE);
    }
    int queue_id = msgget(key, 0666 | IPC_CREAT);

    if (queue_id == -1) {
        perror("error creating queue");
        exit(EXIT_FAILURE);
    }

    pid_t pid1 = fork();
    if (pid1 == -1) {
        perror("fork failed");
        exit(EXIT_FAILURE);
    }

    if (pid1 == 0) {
        struct message msg1;
        msg1.mtype = 1;
        strcpy(msg1.mtext, "Message from process 1");
        if (msgsnd(queue_id, &msg1, strlen(msg1.mtext) + 1, 0) == -1) {
            perror("error sending message");
            exit(EXIT_FAILURE);
        }
        printf("Process 1 sent message\n");

        // Ожидаем сообщение от второго процесса
        struct message msg2;
        if (msgrcv(queue_id, &msg2, sizeof(msg2.mtext), 2, 0) == -1) {
            perror("error receiving message");
            exit(EXIT_FAILURE);
        }
        printf("Process 1 received message: %s\n", msg2.mtext);

        exit(0);
    }

    pid_t pid2 = fork();
    if (pid2 == -1) {
        perror("Ошибка fork");
        exit(EXIT_FAILURE);
    }

    if (pid2 == 0) {
        // Второй дочерний процесс
        // Ожидаем сообщение от первого процесса
        struct message msg1;
        if (msgrcv(queue_id, &msg1, sizeof(msg1.mtext), 1, 0) == -1) {
            perror("error receiving message");
            exit(EXIT_FAILURE);
        }
        printf("Process 2 received message: %s\n", msg1.mtext);

        // Отправляем сообщение в очередь
        struct message msg2;
        msg2.mtype = 2;
        strcpy(msg2.mtext, "Message from process 2");
        if (msgsnd(queue_id, &msg2, strlen(msg2.mtext) + 1, 0) == -1) {
            perror("error sending message");
            exit(EXIT_FAILURE);
        }
        printf("Process 2 sent message\n");

        exit(EXIT_SUCCESS);
    }

    // Родительский процесс ожидает завершения дочерних процессов
    wait(NULL);
    wait(NULL);

    // Удаляем очередь сообщений
    if (msgctl(queue_id, IPC_RMID, NULL) == -1) {
        perror("error deleting queue");
        exit(EXIT_FAILURE);
    }

    return 0;
}
