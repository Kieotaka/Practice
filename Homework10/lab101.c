#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {
    pid_t pid;

    pid = fork();

    if (pid < 0) {
        perror("fork failed");
        return 1;
    } else if (pid == 0) {
        printf("Дочерний процесс: PID = %d, PPID = %d\n", getpid(), getppid());
        return 0;
    } else {
        printf("Родительский процесс: PID = %d, PPID = %d\n", getpid(), getppid());

        int status;
        wait(&status);

        if (WIFEXITED(status)) {
            printf("Дочерний процесс завершился с кодом: %d\n", WEXITSTATUS(status));
        } else if (WIFSIGNALED(status)) {
            printf("Дочерний процесс был завершен сигналом: %d\n", WTERMSIG(status));
        } else {
            printf("Дочерний процесс завершился необычным образом.\n");
        }
    }

    return 0;
}