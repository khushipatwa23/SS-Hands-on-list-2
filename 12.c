
/* Program to create an orphan process by sending SIGKILL to the parent from the child */

#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>

int main() {
    pid_t pid = fork();

    if (pid < 0) {
        perror("fork");
        return 1;
    }

    if (pid == 0) {
        sleep(2);

        pid_t ppid = getppid();
        printf("Child: sending SIGKILL to parent PID %d\n", (int)ppid);
        fflush(stdout);

        if (kill(ppid, SIGKILL) == -1) {
            perror("kill");
            return 1;
        }

        sleep(1);
        printf("Child: new parent PID is %d\n", (int)getppid());
    } else {
        printf("Parent: PID %d, child PID %d\n",
               (int)getpid(), (int)pid);
        fflush(stdout);

        while (1)
            pause();
    }

    return 0;
}
