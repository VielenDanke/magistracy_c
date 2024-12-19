#include <stdio.h>
#include <sys/_types/_key_t.h>

#include "../../../../../../Library/Developer/CommandLineTools/SDKs/MacOSX.sdk/usr/include/stdlib.h"
#include "../../../../../../Library/Developer/CommandLineTools/SDKs/MacOSX.sdk/usr/include/unistd.h"
#include "../../../../../../Library/Developer/CommandLineTools/SDKs/MacOSX.sdk/usr/include/sys/ipc.h"
#include "../../../../../../Library/Developer/CommandLineTools/SDKs/MacOSX.sdk/usr/include/sys/msg.h"
#include "../../../../../../Library/Developer/CommandLineTools/SDKs/MacOSX.sdk/usr/include/sys/_types/_pid_t.h"
//
// Created by Vladislav Dankevich on 19.12.2024.
//
#define MAX_MSG_SIZE 1024

int msqid;

// Структура сообщения
struct message {
    long mtype;
    char mtext[MAX_MSG_SIZE];
};

void sigint_handler(int sig) {
    printf("SIGINT received\n");

    if (msgctl(msqid, IPC_RMID, NULL) == -1) {
        perror("error deleting queue");
    } else {
        printf("Queue deleted\n");
    }
    exit(0);
}

int main() {
    key_t key;
    char *path_to_client_key_file =
            "/Users/vladislavdankevich/CLionProjects/untitled/os_architecture/lab_5/queue_task_7";

    if ((key = ftok(path_to_client_key_file, 72)) == -1) {
        perror("ftok");
        exit(1);
    }

    if ((msqid = msgget(key, 0666 | IPC_CREAT)) == -1) {
        perror("msgget");
        exit(1);
    }
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
                if (msgrcv(msqid, &msg, sizeof(msg.mtext), type, MSG_NOERROR) == -1) {
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
        for (long i = 100; i < 110; i++) {
            struct message msg = {i, "Hello from server"};

            if (msgsnd(msqid, &msg, sizeof(msg.mtext), 0) == -1) {
                perror("msgsnd");
                exit(EXIT_FAILURE);
            }
        }
        sleep(5);
    }
}
