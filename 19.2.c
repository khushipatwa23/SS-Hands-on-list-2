
/* Program: Create a named FIFO called fifo_mkfifo using the
   mkfifo() library function and report whether creation succeeds. */

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>

int main(void) {
    if (mkfifo("fifo_mkfifo", 0666) == -1) {
        perror("mkfifo");
        return EXIT_FAILURE;
    }

    printf("FIFO created successfully using mkfifo().\n");
    return EXIT_SUCCESS;
}
