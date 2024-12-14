//
// Created by Vladislav Dankevich on 14.12.2024.
//
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

struct message {
    long mtype; // message type
    char mtext[100]; // message text
};

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("First argument - message queue id, second argument - message type\n");
        exit(1);
    }
    int msgid = atoi(argv[1]);
    long mtype = atol(argv[2]);

    struct message msg;

    // https://www.opennet.ru/man.shtml?topic=msgrcv&category=2&russian=2
    // IPC_NOWAIT - return immediately if there is no messages
    // MSG_EXCEPT - used with mtype greater than 0, read the first message in the queue with type differs from mtype
    // MSG_NOERROR - to truncate the message text if longer than msgsz bytes
    if (msgrcv(msgid, &msg, sizeof(msg.mtext), mtype, IPC_NOWAIT) == -1) {
        perror("msgrcv");
        exit(1);
    }
    printf("Received message with type %ld: %s\n", msg.mtype, msg.mtext);

    return EXIT_SUCCESS;
}