#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <semaphore.h>
#include <string.h>

#define SHM_NAME "/my_shm"
#define SEM_MSG_READY "/sem_msg_ready"
#define SEM_RESP_READY "/sem_resp_ready"
#define SHM_SIZE 1024

int main() {
    int shm_fd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0666);
    if (shm_fd == -1) { perror("shm_open"); exit(EXIT_FAILURE); }

    if (ftruncate(shm_fd, SHM_SIZE) == -1) { perror("ftruncate"); exit(EXIT_FAILURE); }

    char *shm_ptr = mmap(NULL, SHM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (shm_ptr == MAP_FAILED) { perror("mmap"); exit(EXIT_FAILURE); }

    sem_t *sem_msg_ready = sem_open(SEM_MSG_READY, O_CREAT, 0666, 0);
    sem_t *sem_resp_ready = sem_open(SEM_RESP_READY, O_CREAT, 0666, 0);

    if (sem_msg_ready == SEM_FAILED || sem_resp_ready == SEM_FAILED) {
        perror("sem_open");
        munmap(shm_ptr, SHM_SIZE);
        close(shm_fd);
        shm_unlink(SHM_NAME);
        sem_unlink(SEM_MSG_READY);
        sem_unlink(SEM_RESP_READY);
        exit(EXIT_FAILURE);
    }

    printf("Server: Записываю 'Hi!'\n");
    strcpy(shm_ptr, "Hi!");
    sem_post(sem_msg_ready); 

    printf("Server: Ожидаю ответа...\n");
    sem_wait(sem_resp_ready); 
    printf("Server получил ответ: %s\n", shm_ptr);

  
    munmap(shm_ptr, SHM_SIZE);
    close(shm_fd);
    sem_close(sem_msg_ready);
    sem_close(sem_resp_ready);

   
    sem_unlink(SEM_MSG_READY);
    sem_unlink(SEM_RESP_READY);
    shm_unlink(SHM_NAME);

    return 0;
}