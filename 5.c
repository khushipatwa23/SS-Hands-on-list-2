/* Program to print system limitations using sysconf() */

#include <stdio.h>
#include <unistd.h>
#include <errno.h>

int main() {
    struct {
        const char *name;
        int option;
    } limits[] = {
        {"Maximum argument length for exec", _SC_ARG_MAX},
        {"Maximum processes per user ID", _SC_CHILD_MAX},
        {"Clock ticks per second", _SC_CLK_TCK},
        {"Maximum open files", _SC_OPEN_MAX},
        {"Page size in bytes", _SC_PAGESIZE},
        {"Total physical memory pages", _SC_PHYS_PAGES},
        {"Available physical memory pages", _SC_AVPHYS_PAGES}
    };

    int n = sizeof(limits) / sizeof(limits[0]);

    for (int i = 0; i < n; i++) {
        errno = 0;
        long value = sysconf(limits[i].option);

        if (value == -1) {
            if (errno == 0)
                printf("%s: Limit indeterminate or unavailable\n",
                       limits[i].name);
            else
                perror(limits[i].name);
        } else {
            printf("%s: %ld\n", limits[i].name, value);
        }
    }

    return 0;
}
