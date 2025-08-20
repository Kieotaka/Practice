#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    int pipefd[2];
    if (pipe(pipefd) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }
    pid_t pid = fork();

    if (pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (pid > 0) {
        close(pipefd[0]);
        const char *message = "Hi!";
        write(pipefd[1], message, sizeof("Hi!"));
        close(pipefd[1]);
        wait(NULL);
    } else {
        close(pipefd[1]);
        char buffer[100];
        ssize_t count = read(pipefd[0], buffer, sizeof(buffer) - 1);
        if (count > 0) {
            buffer[count] = '\0';
            printf("%s\n", buffer);
        }
        close(pipefd[0]);
        exit(EXIT_SUCCESS);
    }

    return 0;
}