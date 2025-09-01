#include <ncurses.h>
#include <pthread.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <semaphore.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define SHM_NAME "/chat_shm"
#define SEM_NAME "/chat_sem"
#define MAX_MSG_LEN 256

typedef struct {
    char messages[100][MAX_MSG_LEN];
    int msg_count;
    char users[10][50];
    int user_count;
} shared_data_t;

shared_data_t *shared;
sem_t *sem;

char username[50];
volatile bool running = true; 

void *read_messages(void *arg) {
    int last_msg = 0;
    while (running) {
        sem_wait(sem);
        for (int i = last_msg; i < shared->msg_count; i++) {
            printw("%s\n", shared->messages[i]);
            last_msg++;
        }
        refresh();
        sem_post(sem);
        usleep(500000);
    }
    return NULL;
}

void *send_messages(void *arg) {
    char msg[MAX_MSG_LEN];
    while (running) {
        getstr(msg);
        if (strlen(msg) == 0) continue;

        
        if (strcmp(msg, "/quit") == 0) {
            running = false;
            break;
        }

        sem_wait(sem);
        if (shared->msg_count < 100) {
            snprintf(shared->messages[shared->msg_count], MAX_MSG_LEN, "%s: %s", username, msg);
            shared->msg_count++;
        }
        sem_post(sem);
    }
    return NULL;
}

int main() {
    initscr();
    cbreak();
    noecho();
    int shm_fd = shm_open(SHM_NAME, O_RDWR, 0666);
    shared = mmap(NULL, sizeof(shared_data_t), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    sem = sem_open(SEM_NAME, 0);
    printw("Введите ваше имя: ");
    getstr(username);
    sem_wait(sem);
    if (shared->user_count < 10) {
        strcpy(shared->users[shared->user_count], username);
        shared->user_count++;
        if (shared->msg_count < 100) {
            snprintf(shared->messages[shared->msg_count], MAX_MSG_LEN, "%s присоединился к чату", username);
            shared->msg_count++;
        }
    }
    sem_post(sem);

    pthread_t reader_thread, writer_thread;
    pthread_create(&reader_thread, NULL, read_messages, NULL);
    pthread_create(&writer_thread, NULL, send_messages, NULL);

    pthread_join(writer_thread, NULL);
    running = false;
    pthread_join(reader_thread, NULL);
    endwin();

    munmap(shared, sizeof(shared_data_t));
    close(shm_fd);
    return 0;
}