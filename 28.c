
/* Program: Change the permissions of a System V message queue. */

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

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
        perror("msgctl IPC_STAT");
        return 1;
    }

    printf("Permissions before change: %03o\n",
           (unsigned int)(ds.msg_perm.mode & 0777));

    ds.msg_perm.mode = (ds.msg_perm.mode & ~0777) | 0600;

    if (msgctl(msqid, IPC_SET, &ds) == -1) {
        perror("msgctl IPC_SET");
        return 1;
    }

    if (msgctl(msqid, IPC_STAT, &ds) == -1) {
        perror("msgctl IPC_STAT");
        return 1;
    }

    printf("Permissions after change:  %03o\n",
           (unsigned int)(ds.msg_perm.mode & 0777));

    return 0;
}
