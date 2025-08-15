#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {
    pid_t pid1, pid2;
    pid1 = fork();

    if (pid1 < 0) {
        perror("fork process1 failed");
        return 1;
    }

    if (pid1 == 0) {
        printf("Процесс1: PID = %d, PPID = %d\n", getpid(), getppid());
        pid_t pid3 = fork();
        if (pid3 < 0) {
            perror("fork process3 failed");
            return 1;
        }
        if (pid3 == 0) {
            printf("Процесс3: PID = %d, PPID = %d\n", getpid(), getppid());
            return 0;
        }
        pid_t pid4 = fork();
        if (pid4 < 0) {
            perror("fork process4 failed");
            return 1;
        }
        if (pid4 == 0) {
            printf("Процесс4: PID = %d, PPID = %d\n", getpid(), getppid());
            return 0;
        }
        int status;
        wait(&status); 
        wait(&status);
        
        printf("Процесс1 завершился.\n");
        return 0;
    } else {
        pid2 = fork();

        if (pid2 < 0) {
            perror("fork process2 failed");
            return 1;
        }

        if (pid2 == 0) {
            printf("Процесс2: PID = %d, PPID = %d\n", getpid(), getppid());
            pid_t pid5 = fork();
            if (pid5 < 0) {
                perror("fork process5 failed");
                return 1;
            }
            if (pid5 == 0) {
                printf("Процесс5: PID = %d, PPID = %d\n", getpid(), getppid());
                return 0;
            }
            int status;
            wait(&status);
            
            printf("Процесс2 завершился.\n");
            return 0;

        } else {
            int status;
            wait(&status);
            wait(&status);
            
            printf("Родительский процесс завершился.\n");
        }
    }

    return 0;
}