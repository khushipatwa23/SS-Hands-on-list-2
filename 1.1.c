/* Program: Set an ITIMER_REAL interval timer.
   Initial expiration: 10 seconds
   Repeating interval: 10 microseconds */
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/time.h>
#include <unistd.h>

void handler(int sig) {
    write(STDOUT_FILENO,"ITIMER_REAL timer expired\n",26);
}

int main() {
    struct itimerval timer;

    signal(SIGALRM, handler);

    timer.it_value.tv_sec = 10;
    timer.it_value.tv_usec = 0;

    timer.it_interval.tv_sec = 0;
    timer.it_interval.tv_usec = 10;

if (setitimer(ITIMER_REAL, &timer, NULL) == -1) {
        perror("setitimer");
        return 1;
    }

    while (1) {
        pause();
    }

    return 0;
}
