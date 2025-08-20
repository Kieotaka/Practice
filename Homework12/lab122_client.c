#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

#define FIFO_PATH "/tmp/my_fifo"

int main() {
    int fd = open(FIFO_PATH, O_RDONLY);
    if (fd == -1) {
        perror("open");
        exit(EXIT_FAILURE);
    }

    char buffer[100];
    ssize_t count = read(fd, buffer, sizeof(buffer) - 1);
    if (count > 0) {
        buffer[count] = '\0';
        printf("Received message: %s\n", buffer);
    } else {
        perror("read");
    }

    close(fd);
    return 0;
}