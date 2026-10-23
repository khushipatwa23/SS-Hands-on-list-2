
/* Program: Create shared memory, write data, attach read-only,
   test overwriting, detach, and remove the shared memory. */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

int main(void) {
    int shmid = shmget(IPC_PRIVATE, 1024, IPC_CREAT | 0600);

    if (shmid == -1) {
        perror("shmget");
        return 1;
    }

    char *ptr = (char *)shmat(shmid, NULL, 0);

    if (ptr == (void *)-1) {
        perror("shmat");
        shmctl(shmid, IPC_RMID, NULL);
        return 1;
    }

    strcpy(ptr, "Hello from shared memory");
    printf("Data written: %s\n", ptr);

    if (shmdt(ptr) == -1) {
        perror("shmdt");
        shmctl(shmid, IPC_RMID, NULL);
        return 1;
    }

    /* Attach the same segment in read-only mode. */
    char *readonly_ptr = (char *)shmat(shmid, NULL, SHM_RDONLY);

    if (readonly_ptr == (void *)-1) {
        perror("shmat read-only");
        shmctl(shmid, IPC_RMID, NULL);
        return 1;
    }

    printf("Data read: %s\n", readonly_ptr);
    printf("Attempting to overwrite read-only shared memory...\n");
    fflush(stdout);

    /* A child process tests the write so the main program can
       continue and clean up even if the write causes SIGSEGV. */
    pid_t pid = fork();

    if (pid == -1) {
        perror("fork");
        shmdt(readonly_ptr);
        shmctl(shmid, IPC_RMID, NULL);
        return 1;
    }

    if (pid == 0) {
        readonly_ptr[0] = 'X';
        _exit(0);
    }

    int status;
    if (waitpid(pid, &status, 0) == -1) {
        perror("waitpid");
        shmdt(readonly_ptr);
        shmctl(shmid, IPC_RMID, NULL);
        return 1;
    }

    if (WIFSIGNALED(status) && WTERMSIG(status) == SIGSEGV) {
        printf("Overwrite failed: read-only memory caused SIGSEGV.\n");
    } else if (WIFEXITED(status) && WEXITSTATUS(status) == 0) {
        printf("Overwrite unexpectedly succeeded.\n");
    } else {
        printf("Overwrite test ended abnormally.\n");
    }

    if (shmdt(readonly_ptr) == -1) {
        perror("shmdt read-only");
        shmctl(shmid, IPC_RMID, NULL);
        return 1;
    }

    printf("Shared memory detached successfully.\n");

    if (shmctl(shmid, IPC_RMID, NULL) == -1) {
        perror("shmctl IPC_RMID");
        return 1;
    }

    printf("Shared memory removed successfully.\n");

    return 0;
}
