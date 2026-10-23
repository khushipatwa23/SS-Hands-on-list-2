
/* Program: Send a message to a System V message queue.
   Creates the queue if it does not already exist. */

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

    struct message msg;
    msg.mtype = 1;

    printf("Enter a message: ");

    if (fgets(msg.mtext, sizeof(msg.mtext), stdin) == NULL) {
        fprintf(stderr, "Failed to read message\n");
        return 1;
    }

    msg.mtext[strcspn(msg.mtext, "\n")] = '\0';

    if (msgsnd(msqid, &msg, strlen(msg.mtext) + 1, 0) == -1) {
        perror("msgsnd");
        return 1;
    }

    printf("Message sent successfully.\n");
    printf("Queue ID: %d\n", msqid);
    printf("Message type: %ld\n", msg.mtype);
    printf("Message: %s\n", msg.mtext);

    return 0;
}
