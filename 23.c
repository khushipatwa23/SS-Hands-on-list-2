
/* Program: Print the maximum number of open file descriptors
   available to a process using sysconf(), and display the
   capacity of a pipe buffer using fcntl() on Linux. */

#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

int main(void) {
    long max_files = sysconf(_SC_OPEN_MAX);

    if (max_files == -1) {
        perror("sysconf");
    } else {
        printf("Maximum open file descriptors: %ld\n", max_files);
    }

    int fd[2];

    if (pipe(fd) == -1) {
        perror("pipe");
        return EXIT_FAILURE;
    }

    int pipe_size = fcntl(fd[0], F_GETPIPE_SZ);

    if (pipe_size == -1) {
        perror("fcntl F_GETPIPE_SZ");
        close(fd[0]);
        close(fd[1]);
        return EXIT_FAILURE;
    }

    printf("Pipe buffer size: %d bytes\n", pipe_size);

    close(fd[0]);
    close(fd[1]);

    return EXIT_SUCCESS;
}
