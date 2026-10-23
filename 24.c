
/* Program: Create a System V message queue using ftok() and
   msgget(), then print the IPC key and message queue ID. */

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

int main(void) {
    key_t key;
    int msgid;

    key = ftok(".", 'A');

    if (key == (key_t)-1) {
        perror("ftok");
        return EXIT_FAILURE;
    }

    msgid = msgget(key, IPC_CREAT | 0666);

    if (msgid == -1) {
        perror("msgget");
        return EXIT_FAILURE;
    }

    printf("Message queue key: %ld\n", (long)key);
    printf("Message queue ID: %d\n", msgid);

    return EXIT_SUCCESS;
}
