
/* Program to set the soft limit for open files using setrlimit() system call */

#include <stdio.h>
#include <sys/resource.h>

int main() {
    struct rlimit limit;

    if (getrlimit(RLIMIT_NOFILE, &limit) == -1) {
        perror("getrlimit");
        return 1;
    }

    limit.rlim_cur = 50;

    if (setrlimit(RLIMIT_NOFILE, &limit) == -1) {
        perror("setrlimit");
        return 1;
    }

    printf("Open-file soft limit set successfully.\n");

    if (getrlimit(RLIMIT_NOFILE, &limit) == 0) {
        printf("New soft limit: %llu\n",
               (unsigned long long)limit.rlim_cur);
    }

    return 0;
}
