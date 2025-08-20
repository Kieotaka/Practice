#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

#define FIFO_PATH "/tmp/my_fifo"

int main() {
    if (mkfifo(FIFO_PATH, 0666) == -1) {
        perror("mkfifo");
        
    }


    int fd = open(FIFO_PATH, O_WRONLY);
    if (fd == -1) {
        perror("open");
        unlink(FIFO_PATH); 
        exit(EXIT_FAILURE);
    }

    const char *message = "Hi!";
    write(fd, message, sizeof("Hi!"));

    close(fd);
    unlink(FIFO_PATH);

    return 0;
}