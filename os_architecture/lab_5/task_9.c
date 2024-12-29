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

long clients[5];
int client_idx;

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
            "/Users/dankekz/CLionProjects/magistracy_c/os_architecture/lab_5/client_queue_task_9";
    char *path_to_server_key_file =
            "/Users/dankekz/CLionProjects/magistracy_c/os_architecture/lab_5/server_queue_task_9";

    key_t server_key, client_key;

    if ((server_key = ftok(path_to_server_key_file, 71)) == -1) {
        perror("ftok");
        exit(1);
    }
    if ((client_key = ftok(path_to_client_key_file, 72)) == -1) {
        perror("ftok");
        exit(1);
    }

    msgctl(client_key, IPC_RMID, NULL);
    msgctl(server_key, IPC_RMID, NULL);

    if ((server_msqid = msgget(server_key, 0666 | IPC_CREAT)) == -1) {
        perror("msgget");
        exit(1);
    }

    if ((client_msqid = msgget(client_key, 0666 | IPC_CREAT)) == -1) {
        perror("msgget");
        exit(1);
    }

    printf("Server msqid %d\n", server_msqid);
    printf("Client msqid %d\n", client_msqid);

    for (int i = 0; i < 5; i++) {
        pid_t pid = fork();

        if (pid == -1) {
            perror("fork");
            exit(EXIT_FAILURE);
        }
        if (pid == 0) {
            long current_pid = getpid();
            char str[1024]; // Make sure this buffer is large enough!

            int result = sprintf(str, "%ld-text", current_pid);

            if (result < 0) {
                fprintf(stderr, "Error formatting the string.\n");
                return 1; // Handle the error
            }
            struct message serv_msg;

            serv_msg.mtype = 1;

            strcpy(serv_msg.mtext, str);

            if (msgsnd(server_msqid, &serv_msg, sizeof(serv_msg.mtext), 0) == -1) {
                perror("msgsnd");
                exit(EXIT_FAILURE);
            }
            printf("Child process with PID %d started\n", getpid());

            while (1) {
                int msg_text_result = sprintf(str, "Hello from client with pid %ld", current_pid);

                if (msg_text_result < 0) {
                    fprintf(stderr, "Error formatting the string.\n");
                    exit(EXIT_FAILURE); // Handle the error
                }
                struct message to_send;

                to_send.mtype = current_pid;

                strcpy(to_send.mtext, str);

                if (msgsnd(server_msqid, &to_send, sizeof(serv_msg.mtext), 0) == -1) {
                    perror("msgsnd");
                    exit(EXIT_FAILURE);
                }

                struct message msg;
                // msgtype 0 - pick any first message
                // MSG_NOERROR - trim message if it's too big
                if (msgrcv(client_msqid, &msg, sizeof(msg.mtext), current_pid, 0) == -1) {
                    printf("No message exist for client %d", getpid());
                    sleep(1);
                    continue;
                }
                printf("Client %d received message: %s\n", getpid(), msg.mtext);

                sleep(1);
            }
        }
    }
    signal(SIGINT, sigint_handler);

    printf("Parent process with PID %d started\n", getpid());

    while (1) {
        struct message msg;

        if (msgrcv(server_msqid, &msg, sizeof(msg.mtext), 0, IPC_NOWAIT) == -1) {
            printf("No message exist for server\n");
            sleep(2);
            continue;
        }
        if (msg.mtype == 0) {
            printf("Empty message - continue\n");
            sleep(2);
            continue;
        }
        if (msg.mtype == 1) {
            printf("Received client configuration message with %ld mtype and text %s\n", msg.mtype, msg.mtext);

            if (client_idx == sizeof(clients)) {
                printf("Client list is full. Skip adding client\n");
                continue;
            }
            char *ptr = strtok(msg.mtext, "-");

            if (ptr == NULL) {
                fprintf(stderr, "Invalid string format: no delimiter found.\n");
                exit(EXIT_FAILURE); // Indicate an error
            }
            long number = strtol(ptr, &ptr, 10);

            if (*ptr != '\0') {
                fprintf(stderr, "Invalid string format: non-numeric characters found.\n");
                exit(EXIT_FAILURE);
            }
            if (number == 0 && ptr == msg.mtext) {
                fprintf(stderr, "Invalid number: zero length.\n");
                exit(EXIT_FAILURE);
            }
            printf("Setting client type %ld for client idx %d\n", number, client_idx);
            clients[client_idx++] = number;
            sleep(2);
            continue;
        }
        printf("Receive message from client with %ld type and %s message\n", msg.mtype, msg.mtext);

        for (int i = 0; i < 5; i++) {
            if (clients[i] == msg.mtype) {
                printf("Skip sending self message to a client %ld\n", clients[i]);
                continue;
            }
            if (clients[i] == 0) {
                printf("No client was set for index %d. Skip sending\n", i);
                continue;
            }
            struct message new_msg;

            new_msg.mtype = clients[i];

            strcpy(new_msg.mtext, msg.mtext);

            if (msgsnd(client_msqid, &new_msg, sizeof(new_msg.mtext), 0) == -1) {
                perror("msgsnd");
                exit(EXIT_FAILURE);
            }
        }
        sleep(2);
    }
}
