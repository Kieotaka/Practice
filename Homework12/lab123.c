#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAX_CMD_LEN 1024

int main() {
    char line[MAX_CMD_LEN];
    printf("myshell> ");
    if (fgets(line, sizeof(line), stdin) == NULL) {
        return 0;
    }
    line[strcspn(line, "\n")] = 0;
    char *cmd1 = strtok(line, "|");
    char *cmd2 = strtok(NULL, "|");

    if (cmd2 == NULL) {
        system(line);
        return 0;
    }

    while (*cmd1 == ' ') cmd1++;
    while (*cmd2 == ' ') cmd2++;

    int pipefd[2];

    if (pipe(pipefd) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    pid_t pid1 = fork();
    if (pid1 == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (pid1 == 0) {
        close(pipefd[0]); 
        dup2(pipefd[1], STDOUT_FILENO); 
        close(pipefd[1]);
        execlp("/bin/sh", "sh", "-c", cmd1, (char *)NULL);
        perror("execlp");
        exit(EXIT_FAILURE);
    }

    pid_t pid2 = fork();
    if (pid2 == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (pid2 == 0) {
        close(pipefd[1]); 
        dup2(pipefd[0], STDIN_FILENO); 
        close(pipefd[0]);
        execlp("/bin/sh", "sh", "-c", cmd2, (char *)NULL);
        perror("execlp");
        exit(EXIT_FAILURE);
    }
    close(pipefd[0]);
    close(pipefd[1]);
    waitpid(pid1, NULL, 0);
    waitpid(pid2, NULL, 0);

    return 0;
}