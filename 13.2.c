
/* Program to send SIGSTOP to another process using kill() */

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s PID\n", argv[0]);
        return 1;
    }

    pid_t pid = (pid_t)atoi(argv[1]);

    if (kill(pid, SIGSTOP) == -1) {
        perror("kill");
        return 1;
    }

    printf("SIGSTOP sent to PID %d\n", (int)pid);

    return 0;
}
