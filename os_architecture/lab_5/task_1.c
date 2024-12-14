//
// Created by Vladislav Dankevich on 14.12.2024.
//
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#include "../../../../../../Library/Developer/CommandLineTools/SDKs/MacOSX15.1.sdk/usr/include/string.h"

// Структура для сообщения
struct message {
    long mtype; // Тип сообщения
    char mtext[100]; // Текст сообщения
};

int main() {
    key_t key;
    int msgid;

    char *path_to_key_file = "/Users/vladislavdankevich/CLionProjects/untitled/os_architecture/lab_5/progfile";

    // generate key to use in msgget semget shmget
    // https://www.opennet.ru/man.shtml?topic=ftok&russian=0&category=&submit=%F0%CF%CB%C1%DA%C1%D4%D8+man
    // 65 - proj id (lower 8 bits, not 0)
    if ((key = ftok(path_to_key_file, 65)) == -1) {
        perror("ftok");
        exit(1);
    }

    // access existing message queue or create a new one
    // https://www.opennet.ru/man.shtml?topic=msgget&russian=0&category=&submit=%F0%CF%CB%C1%DA%C1%D4%D8+man
    // msgflg (0666) access rights for a message queue
    // msgid - identificator of a message queue
    if ((msgid = msgget(key, 0666 | IPC_CREAT)) == -1) {
        perror("msgget");
        exit(1);
    }

    // send messages
    struct message msg1 = {100, "First message"};
    struct message msg2 = {200, "Second message"};
    struct message msg3 = {300, "This message is bigger"};

    if (msgsnd(msgid, &msg1, sizeof(msg1.mtext), 0) == -1) {
        perror("msgsnd");
        exit(1);
    }
    if (msgsnd(msgid, &msg2, sizeof(msg2.mtext), 0) == -1) {
        perror("msgsnd");
        exit(1);
    }
    if (msgsnd(msgid, &msg3, sizeof(msg3.mtext), 0) == -1) {
        perror("msgsnd");
        exit(1);
    }

    printf("Сообщения отправлены в очередь.\n");

    // fetch statistic
    struct msqid_ds buf;

    if (msgctl(msgid, IPC_STAT, &buf) == -1) {
        perror("msgctl");
        exit(1);
    }

    printf("ID: %d\n", msgid);
    printf("N of messages: %lu\n", buf.msg_qnum);
    printf("Messages size: %lu\n", buf.msg_qbytes);
    printf("PID the latest sender: %d\n", buf.msg_lspid);
    printf("PID the latest receiver: %d\n", buf.msg_lrpid);

    // clean up the queue, remove if we need to clean up the queue
    // if (msgctl(msgid, IPC_RMID, &buf) == -1) {
    //     perror("msgctl");
    //     exit(1);
    // }
    return EXIT_SUCCESS;
}
