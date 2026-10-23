
/* Program: Handle SIGALRM using alarm() */

#include <stdio.h>
#include <signal.h>
#include <unistd.h>

void handler(int sig) {
    const char msg[] = "SIGALRM received\n";
    write(STDOUT_FILENO, msg, sizeof(msg) - 1);
}

int main() {
    signal(SIGALRM, handler);
    alarm(5);
    pause();

    return 0;
}

