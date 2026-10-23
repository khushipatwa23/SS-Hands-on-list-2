
/* Program to ignore SIGINT and then restore its default action using signal() */

#include <stdio.h>
#include <signal.h>
#include <unistd.h>

int main() {
    signal(SIGINT, SIG_IGN);

    printf("SIGINT is ignored for 5 seconds. Press Ctrl+C.\n");
    sleep(5);

    signal(SIGINT, SIG_DFL);

    printf("Default SIGINT action restored. Press Ctrl+C now.\n");

    while (1)
        pause();

    return 0;
}
