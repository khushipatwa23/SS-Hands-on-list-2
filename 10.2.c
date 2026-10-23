
/* Program: Handle SIGINT using sigaction() */

#include <stdio.h>
#include <signal.h>
#include <unistd.h>

volatile sig_atomic_t caught = 0;

void handler(int sig) {
    const char msg[] = "SIGINT received\n";
    write(STDOUT_FILENO, msg, sizeof(msg) - 1);
    caught = 1;
}

int main() {
    struct sigaction sa;

    sa.sa_handler = handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;

    if (sigaction(SIGINT, &sa, NULL) == -1) {
        perror("sigaction");
        return 1;
    }

    while (!caught) {
        sleep(1);
    }

    return 0;
}

