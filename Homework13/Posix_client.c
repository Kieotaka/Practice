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
    mqd_t connect_mq, main_mq_w, main_mq_r;

    connect_mq = mq_open(CONNECT_QUEUE, O_WRONLY);
    if (connect_mq == (mqd_t)-1) {
        perror("mq_open connect");
        exit(EXIT_FAILURE);
    }
    
    const char *msg_connect = "connect";
    
    if (mq_send(connect_mq, msg_connect, strlen(msg_connect)+1, 0) == -1) {
        perror("mq_send connect");
        mq_close(connect_mq);
        exit(EXIT_FAILURE);
    }
    
   printf("Клиент: отправил 'connect'\n");
   mq_close(connect_mq); 
   main_mq_w = mq_open(MAIN_QUEUE, O_WRONLY);
   if (main_mq_w == (mqd_t)-1) {
       perror("mq_open main_w");
       exit(EXIT_FAILURE);
   }
   
   main_mq_r = mq_open(MAIN_QUEUE, O_RDONLY);
   if (main_mq_r == (mqd_t)-1) {
       perror("mq_open main_r");
       mq_close(main_mq_w);
       exit(EXIT_FAILURE);
   }
   
   char buffer[MAX_SIZE];
   ssize_t bytes_received;

   printf("Клиент: ожидает 'Hi' от сервера...\n");
   bytes_received = mq_receive(main_mq_r, buffer, MAX_SIZE, NULL);

   if (bytes_received >= 0) {
       buffer[bytes_received] = '\0';
       printf("Клиент: получил '%s'\n", buffer);

       if (strcmp(buffer, "Hi") == 0) {
           const char *msg_hello = "Hello";
           if (mq_send(main_mq_w, msg_hello, strlen(msg_hello)+1, 0) == -1) {
               perror("mq_send Hello");
           } else {
               printf("Клиент: отправил 'Hello'\n");
           }
       } else {
           printf("Клиент: неожиданное сообщение\n");
       }
       
   } else {
       perror("mq_receive Hi");
   }
   mq_close(main_mq_w);
   mq_close(main_mq_r);

   return 0;
}