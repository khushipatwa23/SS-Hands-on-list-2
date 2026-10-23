
/* Program: Receive a message using msgrcv() with flags = 0. */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

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

    struct msqid_ds ds;
    if (msgctl(msqid, IPC_STAT, &ds) == -1) {
        perror("msgctl");
        return 1;
    }

    /* Add a sample message if the queue is empty. */
    if (ds.msg_qnum == 0) {
        struct message sample;
        sample.mtype = 1;
        strcpy(sample.mtext, "Sample message");

        if (msgsnd(msqid, &sample, strlen(sample.mtext) + 1, 0) == -1) {
            perror("msgsnd");
            return 1;
        }
    }

    struct message msg;
    ssize_t n = msgrcv(msqid, &msg, sizeof(msg.mtext), 0, 0);

    if (n == -1) {
        perror("msgrcv");
        return 1;
    }

    printf("Received message: %s\n", msg.mtext);
    printf("Message type: %ld\n", msg.mtype);

    return 0;
}
