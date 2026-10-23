
/* Program to send data from parent to child using a pipe */

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

int main() {
    int fd[2];
    char buffer[100];
    char message[] = "Message from parent to child";

    if (pipe(fd) == -1) {
        perror("pipe");
        return 1;
    }

    pid_t pid = fork();

    if (pid < 0) {
        perror("fork");
        return 1;
    }

    if (pid > 0) {
        close(fd[0]);

        write(fd[1], message, strlen(message) + 1);
        close(fd[1]);

        wait(NULL);
    } else {
        close(fd[1]);

        read(fd[0], buffer, sizeof(buffer));
        printf("Child received: %s\n", buffer);

        close(fd[0]);
    }

    return 0;
}
