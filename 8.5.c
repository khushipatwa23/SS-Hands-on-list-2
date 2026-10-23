
/* Program: Handle SIGALRM using ITIMER_REAL */

#include <stdio.h>
#include <signal.h>
#include <sys/time.h>
#include <unistd.h>

void handler(int sig) {
    const char msg[] = "ITIMER_REAL expired\n";
    write(STDOUT_FILENO, msg, sizeof(msg) - 1);
}

int main() {
    struct itimerval timer;

    signal(SIGALRM, handler);

    timer.it_value.tv_sec = 2;
    timer.it_value.tv_usec = 0;
    timer.it_interval.tv_sec = 0;
    timer.it_interval.tv_usec = 0;

    if (setitimer(ITIMER_REAL, &timer, NULL) == -1) {
        perror("setitimer");
        return 1;
    }

    pause();
    return 0;
}

