#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mqueue.h>
#include <fcntl.h>
#include <sys/stat.h>

#define CONNECT_QUEUE "/connect_queue"
#define MAIN_QUEUE "/main_queue"
#define MAX_SIZE 1024

int main() {
    mqd_t connect_mq = (mqd_t)-1, main_mq = (mqd_t)-1;
    struct mq_attr attr;
    char buffer[MAX_SIZE];
    attr.mq_flags = 0;
    attr.mq_maxmsg = 10;
    attr.mq_msgsize = MAX_SIZE;
    attr.mq_curmsgs = 0;


    connect_mq = mq_open(CONNECT_QUEUE, O_CREAT | O_RDONLY, 0644, &attr);
    if (connect_mq == (mqd_t)-1) {
        perror("mq_open connect");
        exit(EXIT_FAILURE);
    }

    printf("Сервер: ожидает подключение...\n");
    ssize_t bytes_read = mq_receive(connect_mq, buffer, MAX_SIZE, NULL);
    if (bytes_read >= 0) {
        buffer[bytes_read] = '\0';
        if (strcmp(buffer, "connect") == 0) {
            printf("Сервер: клиент подключился.\n");
        } else {
            printf("Сервер: получено неожиданное сообщение: %s\n", buffer);
            mq_close(connect_mq);
            mq_unlink(CONNECT_QUEUE);
            exit(EXIT_FAILURE);
        }
    } else {
        perror("mq_receive connect");
        mq_close(connect_mq);
        mq_unlink(CONNECT_QUEUE);
        exit(EXIT_FAILURE);
    }
    mq_close(connect_mq);
    mq_unlink(CONNECT_QUEUE); 

    main_mq = mq_open(MAIN_QUEUE, O_CREAT | O_RDWR, 0644, &attr);
    if (main_mq == (mqd_t)-1) {
        perror("mq_open main");
        mq_unlink(MAIN_QUEUE);
        exit(EXIT_FAILURE);
    }
    const char *msg_hi = "Hi";
    if (mq_send(main_mq, msg_hi, strlen(msg_hi) + 1, 0) == -1) {
        perror("mq_send Hi");
        mq_close(main_mq);
        mq_unlink(MAIN_QUEUE);
        exit(EXIT_FAILURE);
    }
    printf("Сервер: отправил 'Hi'\n");
    printf("Сервер: ожидает 'Hello' от клиента...\n");
    ssize_t r_bytes = mq_receive(main_mq, buffer, MAX_SIZE, NULL);
    if (r_bytes >= 0) {
        buffer[r_bytes] = '\0';
        printf("Сервер: получил сообщение: %s\n", buffer);

        if (strcmp(buffer, "Hello") == 0) {
            printf("Сервер: соединение закрыто.\n");

            mq_close(main_mq);
            mq_unlink(MAIN_QUEUE);
        } else {
            printf("Сервер: неожиданное сообщение.\n");
            mq_close(main_mq);
        }
    } else {
        perror("mq_receive main");
        mq_close(main_mq);
    }

    return 0;
}