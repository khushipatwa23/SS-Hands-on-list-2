
/* Program: Create and initialize a binary System V semaphore. */

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>

union semun {
    int val;
    struct semid_ds *buf;
    unsigned short *array;
};

int main(void) {
    int semid = semget(IPC_PRIVATE, 1, IPC_CREAT | 0600);

    if (semid == -1) {
        perror("semget");
        return 1;
    }

    union semun arg;
    arg.val = 1;

    if (semctl(semid, 0, SETVAL, arg) == -1) {
        perror("semctl SETVAL");
        semctl(semid, 0, IPC_RMID);
        return 1;
    }

    int value = semctl(semid, 0, GETVAL);

    if (value == -1) {
        perror("semctl GETVAL");
        semctl(semid, 0, IPC_RMID);
        return 1;
    }

    printf("Binary semaphore created successfully.\n");
    printf("Semaphore ID: %d\n", semid);
    printf("Initial value: %d\n", value);

    if (semctl(semid, 0, IPC_RMID) == -1) {
        perror("semctl IPC_RMID");
        return 1;
    }

    printf("Semaphore removed successfully.\n");

    return 0;
}
