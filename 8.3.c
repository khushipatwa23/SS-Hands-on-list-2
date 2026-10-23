
/* Program: Handle SIGFPE using signal() */

#include <stdio.h>
#include <signal.h>
#include <unistd.h>

void handler(int sig) {
    const char msg[] = "SIGFPE received\n";
    write(STDOUT_FILENO, msg, sizeof(msg) - 1);
}

int main() {
    signal(SIGFPE, handler);
    raise(SIGFPE);
    return 0;
}

