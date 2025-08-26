#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>

#define KEY 0x12345
#define MAX_MSG_SIZE 512
#define MAX_CLIENTS 100

enum msg_type {
    SERVICE = 1,
    CHAT = 2
};

struct message {
    long mtype;
    char mtext[MAX_MSG_SIZE];
};
typedef struct {
    int qid;            
    char name[50];    
    int active;         
} client_t;

client_t clients[MAX_CLIENTS];


void broadcast_message(const char *text, long mtype) {
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (clients[i].active) {
            struct message msg;
            msg.mtype = mtype;
            snprintf(msg.mtext, MAX_MSG_SIZE, "%s", text);
            msgsnd(clients[i].qid, &msg, strlen(msg.mtext)+1, 0);
        }
    }
}

int main() {
    int qid;
    if ((qid = msgget(KEY, IPC_CREAT | 0666)) == -1) {
        perror("msgget");
        exit(EXIT_FAILURE);
    }

    printf("Сервер запущен. Ожидание клиентов...\n");
    for (int i = 0; i < MAX_CLIENTS; i++) {
        clients[i].active = 0;
        clients[i].qid = -1;
        memset(clients[i].name, 0, sizeof(clients[i].name));
    }

    while (1) {
        struct message msg;
        ssize_t ret = msgrcv(qid, &msg, sizeof(msg.mtext), 0, 0);
        if (ret == -1) {
            perror("msgrcv");
            continue;
        }

        if (msg.mtype == SERVICE) {
            if (strncmp(msg.mtext, "NEW:", 4) == 0) {
                char *token = strtok(msg.mtext + 4, ":");
                if (!token) continue;
                char name[50];
                strncpy(name, token, sizeof(name));

                token = strtok(NULL, ":");
                if (!token) continue;
                int client_qid = atoi(token);
                int slot = -1;
                for (int i = 0; i < MAX_CLIENTS; i++) {
                    if (!clients[i].active) {
                        slot = i;
                        break;
                    }
                }
                if (slot == -1) {
                    printf("Достигнуто максимальное число клиентов.\n");
                    continue;
                }

                clients[slot].qid = client_qid;
                strncpy(clients[slot].name, name, sizeof(clients[slot].name));
                clients[slot].active = 1;

                printf("Новый участник: %s (qID=%d)\n", name, client_qid);
                char notification[MAX_MSG_SIZE];
                snprintf(notification, sizeof(notification), "Пользователь %s присоединился к чату.", name);
                broadcast_message(notification, SERVICE);
            }
        } else if (msg.mtype == CHAT) {
            printf("Сообщение: %s\n", msg.mtext);
            broadcast_message(msg.mtext, CHAT);
        }
    }

    return 0;
}