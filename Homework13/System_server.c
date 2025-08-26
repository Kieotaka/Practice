#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#define KEY_CONNECT 0x1234
#define KEY_MAIN 0x5678

#define MAX_SIZE 1024

struct msgbuf {
    long mtype;
    char mtext[MAX_SIZE];
};

int main() {
    int connect_qid, main_qid;
    struct msgbuf msg;

    connect_qid = msgget(KEY_CONNECT, IPC_CREAT | 0666);
    if (connect_qid == -1) {
        perror("msgget connect");
        exit(EXIT_FAILURE);
    }

    printf("Сервер: ожидает подключение...\n");

    if (msgrcv(connect_qid, &msg, sizeof(msg.mtext), 1, 0) == -1) {
        perror("msgrcv connect");
        msgctl(connect_qid, IPC_RMID, NULL);
        exit(EXIT_FAILURE);
    }

    if (strcmp(msg.mtext, "connect") == 0) {
        printf("Сервер: клиент подключился.\n");
    } else {
        printf("Сервер: получено неожиданное сообщение: %s\n", msg.mtext);
        msgctl(connect_qid, IPC_RMID, NULL);
        exit(EXIT_FAILURE);
    }

    msgctl(connect_qid, IPC_RMID, NULL);

    main_qid = msgget(KEY_MAIN, IPC_CREAT | 0666);
    if (main_qid == -1) {
        perror("msgget main");
        exit(EXIT_FAILURE);
    }
    strcpy(msg.mtext, "Hi");
    msg.mtype = 1;
    if (msgsnd(main_qid, &msg, strlen(msg.mtext)+1, 0) == -1) {
        perror("msgsnd Hi");
        msgctl(main_qid, IPC_RMID, NULL);
        exit(EXIT_FAILURE);
    }
    printf("Сервер: отправил 'Hi'\n");

    if (msgrcv(main_qid, &msg, sizeof(msg.mtext), 2, 0) == -1) {
        perror("msgrcv Hello");
        msgctl(main_qid, IPC_RMID, NULL);
        exit(EXIT_FAILURE);
    }
    
    printf("Сервер: получил сообщение: %s\n", msg.mtext);

    if (strcmp(msg.mtext, "Hello") == 0) {
        printf("Сервер: соединение закрыто.\n");

        msgctl(main_qid, IPC_RMID, NULL);
    } else {
        printf("Сервер: неожиданное сообщение.\n");

        msgctl(main_qid, IPC_RMID, NULL);
    }

    return 0;
}