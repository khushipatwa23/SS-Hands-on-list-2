
/* Program: Communicate bidirectionally with Program A using two
   named FIFOs. Program B reads A's message through fifo_ab and
   sends its reply through fifo_ba. */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

int main(void) {
    char buffer[200];
    struct stat st;

    // Wait until Program A creates both FIFOs.
    while (access("fifo_ab", F_OK) == -1 ||
           access("fifo_ba", F_OK) == -1) {
        sleep(1);
    }

    if (stat("fifo_ab", &st) == -1 || !S_ISFIFO(st.st_mode) ||
        stat("fifo_ba", &st) == -1 || !S_ISFIFO(st.st_mode)) {
        fprintf(stderr, "Required FIFO files are invalid.\n");
        return EXIT_FAILURE;
    }

    int in = open("fifo_ab", O_RDONLY);
    if (in == -1) {
        perror("open fifo_ab");
        return EXIT_FAILURE;
    }

    int out = open("fifo_ba", O_WRONLY);
    if (out == -1) {
        perror("open fifo_ba");
        close(in);
        return EXIT_FAILURE;
    }

    ssize_t n = read(in, buffer, sizeof(buffer) - 1);

    if (n == -1) {
        perror("read");
        close(in);
        close(out);
        return EXIT_FAILURE;
    }

    if (n == 0) {
        printf("No message received from A.\n");
        close(in);
        close(out);
        return EXIT_SUCCESS;
    }

    buffer[n] = '\0';
    printf("Message from A: %s\n", buffer);

    printf("Program B - Enter reply: ");
    fflush(stdout);

    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        close(in);
        close(out);
        return EXIT_FAILURE;
    }

    size_t len = strlen(buffer);

    if (write(out, buffer, len) == -1) {
        perror("write");
        close(in);
        close(out);
        return EXIT_FAILURE;
    }

    close(in);
    close(out);

    return EXIT_SUCCESS;
}
