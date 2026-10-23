
/* Program: Receive a message using msgrcv() with IPC_NOWAIT. */

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <errno.h>

struct message {
    long mtype;
    char mtext[100];
};

int main(void) {
    key_t key = ftok(".", 'Q');
    if (key == -1) {
        perror("ftok");
        return 1;
    }

    int msqid = msgget(key, IPC_CREAT | 0666);
    if (msqid == -1) {
        perror("msgget");
        return 1;
    }

    struct message msg;

    ssize_t n = msgrcv(msqid, &msg, sizeof(msg.mtext),
                       0, IPC_NOWAIT);

    if (n == -1) {
        if (errno == ENOMSG) {
            printf("No message available in the queue.\n");
            return 0;
        }

        perror("msgrcv");
        return 1;
    }

    printf("Received message: %s\n", msg.mtext);
    printf("Message type: %ld\n", msg.mtype);

    return 0;
}
