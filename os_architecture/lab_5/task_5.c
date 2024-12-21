//
// Created by Vladislav Dankevich on 19.12.2024.
//
#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#include "../../../../../../Library/Developer/CommandLineTools/SDKs/MacOSX.sdk/usr/include/string.h"
#include "../../../../../../Library/Developer/CommandLineTools/SDKs/MacOSX.sdk/usr/include/sys/errno.h"

/*
*IPC_RMID
Немедленно удалить очередь сообщений и связанную с ним структуру данных,
"разбудив" все процессы, ожидающие записи или чтения этой очереди
(при этом функция возвращает ошибку, а переменная errno приобретает значение EIDRM).
Вызывающий процесс должен иметь соответствующие (вероятнее всего для root)
привилегии или его действующий идентификатор пользователя
должен соответствовать создателю или владельцу очереди сообщений.
*/
int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Использование: %s <ключ_очереди>\n", argv[0]);
        return 1;
    }

    int msqid = atoi(argv[1]);

    if (msgctl(msqid, IPC_RMID, NULL) == -1) {
        perror("Error removing message queue");
        return 1;
    }

    return 0;
}
