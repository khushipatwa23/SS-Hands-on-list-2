
/* Program for two-way communication between parent and child using two pipes */

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

int main() {
    int p2c[2], c2p[2];
    char buffer[100];

    if (pipe(p2c) == -1 || pipe(c2p) == -1) {
        perror("pipe");
        return 1;
    }

    pid_t pid = fork();

    if (pid < 0) {
        perror("fork");
        return 1;
    }

    if (pid > 0) {
        close(p2c[0]);
        close(c2p[1]);

        char message[] = "Hello from parent";
        write(p2c[1], message, strlen(message) + 1);
        close(p2c[1]);

        read(c2p[0], buffer, sizeof(buffer));
        printf("Parent received: %s\n", buffer);

        close(c2p[0]);
        wait(NULL);
    } else {
        close(p2c[1]);
        close(c2p[0]);

        read(p2c[0], buffer, sizeof(buffer));
        printf("Child received: %s\n", buffer);
        close(p2c[0]);

        char reply[] = "Hello from child";
        write(c2p[1], reply, strlen(reply) + 1);
        close(c2p[1]);
    }

    return 0;
}
