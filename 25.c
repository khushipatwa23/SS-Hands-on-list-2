
/* Program: Display information about a System V message queue.
   Creates the queue if it does not already exist. */

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <errno.h>

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

    printf("Message Queue Information\n");
    printf("-------------------------\n");
    printf("Key:                 %ld\n", (long)key);
    printf("Queue ID:            %d\n", msqid);
    printf("Owner UID:           %u\n", (unsigned int)ds.msg_perm.uid);
    printf("Owner GID:           %u\n", (unsigned int)ds.msg_perm.gid);
    printf("Permissions:         %03o\n",
           (unsigned int)(ds.msg_perm.mode & 0777));
    printf("Number of messages:  %lu\n",
           (unsigned long)ds.msg_qnum);
    printf("Maximum queue bytes: %lu\n",
           (unsigned long)ds.msg_qbytes);
    printf("Last send PID:       %d\n", (int)ds.msg_lspid);
    printf("Last receive PID:    %d\n", (int)ds.msg_lrpid);
    printf("Last send time:      %ld\n", (long)ds.msg_stime);
    printf("Last receive time:   %ld\n", (long)ds.msg_rtime);
    printf("Last change time:    %ld\n", (long)ds.msg_ctime);

    return 0;
}
