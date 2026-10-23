
/* Program: Create a named FIFO called fifo_mknod using the
   mknod() system call and report whether creation succeeds. */

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>

int main(void) {
    if (mknod("fifo_mknod", S_IFIFO | 0666, 0) == -1) {
        perror("mknod");
        return EXIT_FAILURE;
    }

    printf("FIFO created successfully using mknod().\n");
    return EXIT_SUCCESS;
}
