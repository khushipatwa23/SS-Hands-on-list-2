
/* Program: Use a counting semaphore to protect two pseudo-resources. */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/wait.h>

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
    arg.val = 2;

    if (semctl(semid, 0, SETVAL, arg) == -1) {
        perror("semctl SETVAL");
        semctl(semid, 0, IPC_RMID);
        return 1;
    }

    for (int i = 1; i <= 3; i++) {
        pid_t pid = fork();

        if (pid == -1) {
            perror("fork");
            semctl(semid, 0, IPC_RMID);
            return 1;
        }

        if (pid == 0) {
            struct sembuf acquire = {0, -1, 0};
            struct sembuf release = {0, 1, 0};

            if (semop(semid, &acquire, 1) == -1) {
                perror("semop acquire");
                _exit(1);
            }

            printf("Process %d acquired a resource.\n", i);
            fflush(stdout);

            sleep(2);

            printf("Process %d released its resource.\n", i);
            fflush(stdout);

            if (semop(semid, &release, 1) == -1) {
                perror("semop release");
                _exit(1);
            }

            _exit(0);
        }
    }

    for (int i = 0; i < 3; i++)
        wait(NULL);

    if (semctl(semid, 0, IPC_RMID) == -1) {
        perror("semctl IPC_RMID");
        return 1;
    }

    printf("Counting semaphore removed.\n");
    return 0;
}
