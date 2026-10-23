
/* Program: Protect shared ticket-number creation using a semaphore. */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/wait.h>

union semun {
    int val;
    struct semid_ds *buf;
    unsigned short *array;
};

struct shared_data {
    int ticket;
};

int main(void) {
    int shmid = shmget(IPC_PRIVATE, sizeof(struct shared_data),
                       IPC_CREAT | 0600);
    if (shmid == -1) {
        perror("shmget");
        return 1;
    }

    struct shared_data *data = shmat(shmid, NULL, 0);
    if (data == (void *)-1) {
        perror("shmat");
        shmctl(shmid, IPC_RMID, NULL);
        return 1;
    }
    data->ticket = 0;

    int semid = semget(IPC_PRIVATE, 1, IPC_CREAT | 0600);
    if (semid == -1) {
        perror("semget");
        shmdt(data);
        shmctl(shmid, IPC_RMID, NULL);
        return 1;
    }

    union semun arg;
    arg.val = 1;
    if (semctl(semid, 0, SETVAL, arg) == -1) {
        perror("semctl SETVAL");
        semctl(semid, 0, IPC_RMID);
        shmdt(data);
        shmctl(shmid, IPC_RMID, NULL);
        return 1;
    }

    for (int p = 0; p < 2; p++) {
        pid_t pid = fork();

        if (pid == -1) {
            perror("fork");
            semctl(semid, 0, IPC_RMID);
            shmdt(data);
            shmctl(shmid, IPC_RMID, NULL);
            return 1;
        }

        if (pid == 0) {
            struct sembuf lock = {0, -1, 0};
            struct sembuf unlock = {0, 1, 0};

            for (int i = 0; i < 5; i++) {
                if (semop(semid, &lock, 1) == -1) {
                    perror("semop lock");
                    _exit(1);
                }

                int ticket = ++data->ticket;
                printf("Process %d issued ticket %d\n",
                       (int)getpid(), ticket);
                fflush(stdout);

                if (semop(semid, &unlock, 1) == -1) {
                    perror("semop unlock");
                    _exit(1);
                }

                usleep(10000);
            }
            shmdt(data);
            _exit(0);
        }
    }

    wait(NULL);
    wait(NULL);

    printf("Total tickets issued: %d\n", data->ticket);

    shmdt(data);
    shmctl(shmid, IPC_RMID, NULL);
    semctl(semid, 0, IPC_RMID);

    return 0;
}

