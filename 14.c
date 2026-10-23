
/* Program to create a pipe, write data, read data and display it */

#include <stdio.h>
#include <unistd.h>
#include <string.h>

int main() {
    int fd[2];
    char buffer[100];
    char message[] = "Hello through pipe";

    if (pipe(fd) == -1) {
        perror("pipe");
        return 1;
    }

    write(fd[1], message, strlen(message) + 1);
    read(fd[0], buffer, sizeof(buffer));

    printf("Data read from pipe: %s\n", buffer);

    close(fd[0]);
    close(fd[1]);

    return 0;
}
