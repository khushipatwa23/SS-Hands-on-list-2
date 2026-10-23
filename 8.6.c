
/* Program to catch SIGVTALRM using setitimer() */

#include <stdio.h>
#include <signal.h>
#include <sys/time.h>
#include <unistd.h>

volatile sig_atomic_t caught = 0;

void handler(int sig) {
    caught = 1;
    write(STDOUT_FILENO, "SIGVTALRM caught\n", 17);
}

int main() {
    struct itimerval timer = {0};

    signal(SIGVTALRM, handler);

    timer.it_value.tv_sec = 2;

    if (setitimer(ITIMER_VIRTUAL, &timer, NULL) == -1) {
        perror("setitimer");
        return 1;
    }

    while (!caught) {
    }

    return 0;
}

