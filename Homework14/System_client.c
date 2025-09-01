#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <string.h>
#include <errno.h>

#define SHM_KEY 0x1234
#define SEM_MSG_READY_KEY 0x5678
#define SEM_RESP_READY_KEY 0x9ABC
#define SHM_SIZE 1024
void sem_wait(int semid, int sem_num) {
    struct sembuf sb = {sem_num, -1, 0};
    if (semop(semid, &sb, 1) == -1) {
        perror("semop wait");
        exit(EXIT_FAILURE);
    }
}

void sem_post(int semid, int sem_num) {
    struct sembuf sb = {sem_num, 1, 0};
    if (semop(semid, &sb, 1) == -1) {
        perror("semop post");
        exit(EXIT_FAILURE);
    }
}

int main() {
    int shmid = shmget(SHM_KEY, SHM_SIZE, 0666);
    if (shmid == -1) {
        perror("shmget");
        exit(EXIT_FAILURE);
    }

    char *shm_ptr = shmat(shmid, NULL, 0);
    if (shm_ptr == (void*) -1) {
        perror("shmat");
        exit(EXIT_FAILURE);
    }

    int sem_msg_ready = semget(SEM_MSG_READY_KEY, 1, 0666);
    int sem_resp_ready = semget(SEM_RESP_READY_KEY ,1 ,0666);

    if (sem_msg_ready == -1 || sem_resp_ready == -1) {
        perror("semget");
        shmdt(shm_ptr);
        exit(EXIT_FAILURE);
   }

   printf("Клиент: Жду сообщение...\n");

   sem_wait(sem_msg_ready,0);

   printf("Клиент: Получено сообщение '%s'\n", shm_ptr);

   printf("Клиент: Отправляю ответ 'Hello!'\n");

   strcpy(shm_ptr,"Hello!");
   sem_post(sem_resp_ready,0);
   shmdt(shm_ptr);

   return 0;
}