//
// Created by Vladislav Dankevich on 14.12.2024.
//
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>

struct message {
    long mtype; // message type
    char mtext[128]; // message text
};

int main(int argc, char *argv[]) {
    if (argc != 4) {
        printf("has to provide 3 arguments: msgid, mtype, bytes_to-read");
        exit(1);
    }

    int msqid = atoi(argv[1]);
    long mtype = atol(argv[2]);

    struct message msg;

    // MSG_NOERROR will trim text if it's too big. Other part will be lost
    ssize_t received_bytes = msgrcv(msqid, &msg, sizeof(msg.mtext), mtype, MSG_NOERROR);
    if (received_bytes == -1) {
        perror("msgrcv");
        exit(1);
    }
    printf("Received message type %ld: %s\n", msg.mtype, msg.mtext);

    return EXIT_SUCCESS;
}
