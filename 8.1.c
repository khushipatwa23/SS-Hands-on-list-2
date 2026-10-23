
/* Program: Handle SIGSEGV using signal() */

#include <stdio.h>
#include <signal.h>
#include <unistd.h>

void handler(int sig) {
    const char msg[] = "SIGSEGV received\n";
    write(STDOUT_FILENO, msg, sizeof(msg) - 1);
}

int main() {
    signal(SIGSEGV, handler);
    raise(SIGSEGV);
    return 0;
}

