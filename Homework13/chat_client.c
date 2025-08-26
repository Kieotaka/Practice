#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <ncurses.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>

#define KEY 0x12345
#define MAX_MSG_SIZE 512

enum msg_type {
    SERVICE = 1,
    CHAT = 2
};

struct message {
    long mtype;
    char mtext[MAX_MSG_SIZE];
};

volatile int running = 1;

void *receiver_thread(void *arg) {
    int qid = *(int*)arg;
    WINDOW *chat_win = newwin(20, 80, 0, 0);
    scrollok(chat_win, TRUE);
    wrefresh(chat_win);

    while (running) {
        struct message msg;
        ssize_t ret = msgrcv(qid, &msg, sizeof(msg.mtext), 0, IPC_NOWAIT);
        if (ret > 0) {
            if (msg.mtype == SERVICE) {
                wprintw(chat_win, "[SERVICE] %s\n", msg.mtext);
            } else if (msg.mtype == CHAT) {
                wprintw(chat_win, "%s\n", msg.mtext);
            }
            wrefresh(chat_win);
        } else {
            usleep(100000); 
        }
    }
    delwin(chat_win);
    return NULL;
}

int main() {
    key_t key = KEY;
    int qid;

    initscr();
    cbreak();
    noecho();

    printw("Введите ваше имя: ");
    char username[50];
    getstr(username);

    qid = msgget(key, IPC_CREAT | 0666);
    if (qid == -1) {
        endwin();
        perror("msgget");
        exit(EXIT_FAILURE);
    }


    struct message msg;
    msg.mtype = SERVICE;
    snprintf(msg.mtext, MAX_MSG_SIZE, "NEW:%s", username);
    msgsnd(qid, &msg, strlen(msg.mtext)+1, 0);

    pthread_t tid;
    pthread_create(&tid, NULL, receiver_thread, &qid);

    WINDOW *input_win = newwin(3,80,20,0);

    box(input_win, 0, 0);
    
    wrefresh(input_win);

     while (running) {
         mvwprintw(input_win,1,1,"Введите сообщение ('/quit' для выхода): ");
         wclrtoeol(input_win);
         wrefresh(input_win);

         char input[MAX_MSG_SIZE];
         wgetnstr(input_win, input, MAX_MSG_SIZE-1);

         if (strcmp(input,"/quit") ==0){
             running=0;
             break;
         }

         
         size_t max_username_len = sizeof(((struct message*)NULL)->mtext)/2 - 1; 
         size_t max_input_len = sizeof(((struct message*)NULL)->mtext) - max_username_len - 3; 

         char safe_username[ max_username_len + 1 ];
         char safe_input[ max_input_len + 1 ];

         
         strncpy(safe_username, username, max_username_len);
         safe_username[max_username_len] = '\0';

         
         strncpy(safe_input, input, max_input_len);
         safe_input[max_input_len] = '\0';

         struct message chat_msg;
         chat_msg.mtype=CHAT;

        
         snprintf(chat_msg.mtext,sizeof(chat_msg.mtext),"%s: %s", safe_username,safe_input);

         msgsnd(qid,&chat_msg,sizeof(chat_msg.mtext),0);
     }

     pthread_join(tid,NULL);

     endwin();

     return 0;
}