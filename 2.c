
/* Program to print system resource limits using getrlimit() system call */

#include <stdio.h>
#include <sys/resource.h>

int main() {
    struct rlimit limit;
    int resources[] = {
        RLIMIT_CPU,
        RLIMIT_FSIZE,
        RLIMIT_DATA,
        RLIMIT_STACK,
        RLIMIT_CORE,
        RLIMIT_NOFILE
    };

    char *names[] = {
        "CPU time",
        "File size",
        "Data segment",
        "Stack size",
        "Core file size",
        "Open files"
    };

    int n = sizeof(resources) / sizeof(resources[0]);

    for (int i = 0; i < n; i++) {
        if (getrlimit(resources[i], &limit) == 0) {
            printf("%s:\n", names[i]);

            if (limit.rlim_cur == RLIM_INFINITY)
                printf("  Soft limit: Unlimited\n");
            else
                printf("  Soft limit: %llu\n",
                       (unsigned long long)limit.rlim_cur);

            if (limit.rlim_max == RLIM_INFINITY)
                printf("  Hard limit: Unlimited\n");
            else
                printf("  Hard limit: %llu\n",
                       (unsigned long long)limit.rlim_max);
        } else {
            perror("getrlimit");
        }
    }

    return 0;
}
