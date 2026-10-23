
/* Program to test whether SIGSTOP can be caught */

#include <stdio.h>
#include <signal.h>
#include <unistd.h>

void handler(int sig) {
    printf("SIGSTOP caught\n");
}

int main() {
    signal(SIGSTOP, handler);

    printf("Receiver PID: %d\n", (int)getpid());
    printf("Waiting for signals...\n");
    fflush(stdout);

    while (1)
        pause();

    return 0;
}
