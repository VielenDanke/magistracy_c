#include <stdio.h>
#include <sys/_types/_key_t.h>

#include "../../../../../../Library/Developer/CommandLineTools/SDKs/MacOSX.sdk/usr/include/stdlib.h"
#include "../../../../../../Library/Developer/CommandLineTools/SDKs/MacOSX.sdk/usr/include/string.h"
#include "../../../../../../Library/Developer/CommandLineTools/SDKs/MacOSX.sdk/usr/include/unistd.h"
#include "../../../../../../Library/Developer/CommandLineTools/SDKs/MacOSX.sdk/usr/include/sys/ipc.h"
#include "../../../../../../Library/Developer/CommandLineTools/SDKs/MacOSX.sdk/usr/include/sys/msg.h"
#include "../../../../../../Library/Developer/CommandLineTools/SDKs/MacOSX.sdk/usr/include/sys/_types/_pid_t.h"
//
// Created by Vladislav Dankevich on 19.12.2024.
//
#define MAX_MSG_SIZE 1024

int server_msqid, client_msqid;

// Структура сообщения
struct message {
    long mtype;
    char mtext[MAX_MSG_SIZE];
};

void sigint_handler(int sig) {
    printf("SIGINT received\n");

    if (msgctl(server_msqid, IPC_RMID, NULL) == -1) {
        perror("error deleting queue");
    } else {
        printf("Queue deleted\n");
    }
    if (msgctl(client_msqid, IPC_RMID, NULL) == -1) {
        perror("error deleting queue");
    } else {
        printf("Queue deleted\n");
    }
    exit(0);
}

int main() {
    char *path_to_client_key_file =
            "/Users/vladislavdankevich/CLionProjects/untitled/os_architecture/lab_5/client_queue_task_9";
    char *path_to_server_key_file =
            "/Users/vladislavdankevich/CLionProjects/untitled/os_architecture/lab_5/server_queue_task_9";

    key_t server_key, client_key;

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

    for (long i = 100; i < 110; i++) {
        pid_t pid = fork();

        if (pid == -1) {
            perror("fork");
            exit(EXIT_FAILURE);
        }
        if (pid == 0) {
            const long type = i;
            printf("Child process with PID %d started\n", getpid());

            while (1) {
                struct message msg;

                // msgtype 0 - pick any first message
                // MSG_NOERROR - trim message if it's too big
                if (msgrcv(client_msqid, &msg, sizeof(msg.mtext), type, MSG_NOERROR) == -1) {
                    perror("msgrcv");
                    exit(EXIT_FAILURE);
                }
                printf("Client %d received message: %s\n", getpid(), msg.mtext);
            }
        }
    }
    sleep(2);

    signal(SIGINT, sigint_handler);

    printf("Parent process with PID %d started\n", getpid());

    while (1) {
        struct message msg;

        if (msgrcv(server_msqid, &msg, sizeof(msg.mtext), 0, MSG_NOERROR) == -1) {
            perror("msgrcv");
            exit(EXIT_FAILURE);
        }

        for (long i = 100; i < 110; i++) {
            struct message new_msg = {i};

            strcpy(new_msg.mtext, msg.mtext);

            if (msgsnd(client_msqid, &new_msg, sizeof(new_msg.mtext), 0) == -1) {
                perror("msgsnd");
                exit(EXIT_FAILURE);
            }
        }
        sleep(5);
    }
}
