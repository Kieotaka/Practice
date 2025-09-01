    #include <stdio.h>
    #include <stdlib.h>
    #include <fcntl.h>
    #include <sys/mman.h>
    #include <semaphore.h>
    #include <unistd.h>
    #include <string.h>
    #include <sys/stat.h>

    #define SHM_NAME "/chat_shm"
    #define SEM_NAME "/chat_sem"
    #define MAX_USERS 10
    #define MAX_MSG_LEN 256

    typedef struct {
        char messages[100][MAX_MSG_LEN];
        int msg_count;
        char users[MAX_USERS][50];
        int user_count;
    } shared_data_t;

    int main() {
        int shm_fd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0666);
        ftruncate(shm_fd, sizeof(shared_data_t));
        shared_data_t *shared = mmap(NULL, sizeof(shared_data_t), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);

        memset(shared, 0, sizeof(shared_data_t));

        sem_t *sem = sem_open(SEM_NAME, O_CREAT, 0666, 1);

        printf("Server запущен.\n");

        
        sem_wait(sem);
        if (shared->msg_count < 100) {
            snprintf(shared->messages[shared->msg_count], MAX_MSG_LEN, "Сервер запущен");
            shared->msg_count++;
        }
        sem_post(sem);

        while (1) {
            sleep(1);
        }

        munmap(shared, sizeof(shared_data_t));
        close(shm_fd);
        shm_unlink(SHM_NAME);
        sem_close(sem);
        sem_unlink(SEM_NAME);
        return 0;
    }