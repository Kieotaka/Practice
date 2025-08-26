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

    connect_qid = msgget(KEY_CONNECT, 0666);
    if (connect_qid == -1) {
        perror("msgget connect");
        exit(EXIT_FAILURE);
    }

    struct msgbuf msg;

    strcpy(msg.mtext, "connect");
    msg.mtype = 1;
    
    if (msgsnd(connect_qid, &msg, strlen(msg.mtext)+1, 0) == -1) {
        perror("msgsnd connect");
        exit(EXIT_FAILURE);
    }
    
    printf("Клиент: отправил 'connect'\n");

    main_qid = msgget(KEY_MAIN, 0666);
    
   if (main_qid == -1) {
       perror("msgget main");
       exit(EXIT_FAILURE);
   }

   if (msgrcv(main_qid, &msg, sizeof(msg.mtext), 1, 0) == -1) {
       perror("msgrcv Hi");
       exit(EXIT_FAILURE);
   }
   
   printf("Клиент: получил '%s'\n", msg.mtext);

   if (strcmp(msg.mtext,"Hi") ==0){
       strcpy(msg.mtext,"Hello");
       msg.mtype=2;
       if (msgsnd(main_qid,&msg,sizeof(msg.mtext),0)==-1){
           perror("msgsnd Hello");
           exit(EXIT_FAILURE);
       }
       printf("Клиент: отправил 'Hello'\n");
   } else{
       printf("Клиент: неожиданное сообщение\n");
   }

   return 0;
}