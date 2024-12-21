//
// Created by Vladislav Dankevich on 19.12.2024.
//
// server.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
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
    char *path_to_client_key_file = "/Users/vladislavdankevich/CLionProjects/untitled/os_architecture/lab_5/client_queue";
    char *path_to_server_key_file = "/Users/vladislavdankevich/CLionProjects/untitled/os_architecture/lab_5/server_queue";

    key_t server_key, client_key;
    int server_msqid, client_msqid;

    if ((server_key = ftok(path_to_server_key_file, 71)) == -1) {
        perror("ftok");
        exit(1);
    }

    if ((server_msqid = msgget(server_key, 0666 | IPC_CREAT)) == -1) {
        perror("msgget");
        exit(1);
    }

    if ((client_key = ftok(path_to_client_key_file, 72)) == -1) {
        perror("ftok");
        exit(1);
    }

    if ((client_msqid = msgget(client_key, 0666 | IPC_CREAT)) == -1) {
        perror("msgget");
        exit(1);
    }

    printf("Server msqid %d\n", server_msqid);
    printf("Client msqid %d\n", client_msqid);

    pid_t pid = fork();

    if (pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }
    if (pid == 0) {
        printf("Client process started\n");

        struct message info_message = {1};

        char str[MAX_MSG_SIZE];

        sprintf(str, "%d", client_msqid);

        strcpy(info_message.mtext, str);

        if (msgsnd(server_msqid, &info_message, sizeof(info_message.mtext), 0) == -1) {
            perror("msgsnd");
            exit(EXIT_FAILURE);
        }
        struct message msg;

        while (1) {
            // msgtype 0 - pick any first message
            // MSG_NOERROR - trim message if it's too big
            if (msgrcv(client_msqid, &msg, sizeof(msg.mtext), 0, MSG_NOERROR) == -1) {
                perror("msgrcv");
                exit(EXIT_FAILURE);
            }
            printf("Client received message: %s\n", msg.mtext);
        }
    }
    printf("Server process started\n");

    pid_t new_pid = fork();

    if (new_pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }
    if (new_pid == 0) {
        int message_counter = 0;

        while (1) {
            sleep(5);

            char str[MAX_MSG_SIZE];

            sprintf(str, "Message: %d", message_counter++);

            struct message msg = {100};

            strcpy(msg.mtext, str);

            if (msgsnd(server_msqid, &msg, sizeof(msg.mtext), 0) == -1) {
                perror("msgsnd");
                exit(EXIT_FAILURE);
            }
        }
    }

    struct message info_message;

    if (msgrcv(server_msqid, &info_message, sizeof(info_message.mtext), 1, MSG_NOERROR) == -1) {
        perror("msgrcv");
        exit(EXIT_FAILURE);
    }
    printf("Server received info message: %s\n", info_message.mtext);

    int client_msqid_received = atoi(info_message.mtext);

    while (1) {
        struct message msg;

        // msgtype 100 - pick any first message
        // MSG_NOERROR - trim message if it's too big
        if (msgrcv(server_msqid, &msg, sizeof(msg.mtext), 100, MSG_NOERROR) == -1) {
            perror("msgrcv");
            exit(EXIT_FAILURE);
        }
        if (msgsnd(client_msqid_received, &msg, sizeof(msg.mtext), 0) == -1) {
            perror("msgsnd");
            exit(EXIT_FAILURE);
        }
    }
}