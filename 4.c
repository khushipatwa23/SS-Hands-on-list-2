
/* Program to measure the time taken by 100 getppid() calls using the timestamp counter */

#include <stdio.h>
#include <unistd.h>
#include <stdint.h>

static inline uint64_t rdtsc() {
    unsigned int lo, hi;

    __asm__ volatile ("rdtsc" : "=a"(lo), "=d"(hi));

    return ((uint64_t)hi << 32) | lo;
}

int main() {
    uint64_t start, end;
    pid_t ppid;

    start = rdtsc();

    for (int i = 0; i < 100; i++) {
        ppid = getppid();
    }

    end = rdtsc();

    printf("Parent process ID: %d\n", (int)ppid);
    printf("Timestamp counter difference: %llu cycles\n",
           (unsigned long long)(end - start));

    return 0;
}
