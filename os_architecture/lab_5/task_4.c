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
    char mtext[1024]; // message text
};

int main(int argc, char *argv[]) {
    if (argc != 4) {
        printf("has to provide 3 arguments: msgid, mtype, bytes_to-read");
        exit(1);
    }

    int msgid = atoi(argv[1]);
    long mtype = atol(argv[2]);
    int bytes_to_read = atoi(argv[3]);

    struct message msg;

    // receive msg of size msg.mtext (1024)
    ssize_t received_bytes = msgrcv(msgid, &msg, sizeof(msg.mtext), mtype, 0);
    if (received_bytes == -1) {
        perror("msgrcv");
        exit(1);
    }
    // trim message
    if (received_bytes > bytes_to_read) {
        msg.mtext[bytes_to_read] = '\0';
    } else {
        msg.mtext[received_bytes] = '\0';
    }
    printf("Received message type %ld: %s\n", msg.mtype, msg.mtext);

    return EXIT_SUCCESS;
}