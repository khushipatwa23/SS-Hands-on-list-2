
/* Program to ignore SIGINT and restore its default action using sigaction() */

#include <stdio.h>
#include <signal.h>
#include <unistd.h>

int main() {
    struct sigaction sa = {0};

    sa.sa_handler = SIG_IGN;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sigaction(SIGINT, &sa, NULL);

    printf("SIGINT ignored for 5 seconds. Press Ctrl+C.\n");
    sleep(5);

    sa.sa_handler = SIG_DFL;
    sigaction(SIGINT, &sa, NULL);

    printf("Default action restored. Press Ctrl+C now.\n");

    while (1)
        pause();

    return 0;
}
