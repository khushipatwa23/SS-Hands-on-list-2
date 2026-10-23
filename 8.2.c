
/* Program: Handle SIGINT using signal() */

#include <stdio.h>
#include <signal.h>
#include <unistd.h>

void handler(int sig) {
    const char msg[] = "SIGINT received\n";
    write(STDOUT_FILENO, msg, sizeof(msg) - 1);
}

int main() {
    signal(SIGINT, handler);

    while (1) {
        pause();
    }

    return 0;
}

