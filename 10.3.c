
/* Program to catch SIGFPE using sigaction() */

#include <stdio.h>
#include <signal.h>
#include <unistd.h>

void handler(int sig) {
    write(STDOUT_FILENO, "SIGFPE caught\n", 14);
}

int main() {
    struct sigaction sa = {0};

    sa.sa_handler = handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;

    sigaction(SIGFPE, &sa, NULL);

    raise(SIGFPE);

    return 0;
}
