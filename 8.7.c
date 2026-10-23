
/* Program to catch SIGPROF using setitimer() */

#include <stdio.h>
#include <signal.h>
#include <sys/time.h>
#include <unistd.h>

volatile sig_atomic_t caught = 0;

void handler(int sig) {
    caught = 1;
    write(STDOUT_FILENO, "SIGPROF caught\n", 15);
}

int main() {
    struct itimerval timer = {0};

    signal(SIGPROF, handler);

    timer.it_value.tv_sec = 2;

    if (setitimer(ITIMER_PROF, &timer, NULL) == -1) {
        perror("setitimer");
        return 1;
    }

    while (!caught) {
    }

    return 0;
}

