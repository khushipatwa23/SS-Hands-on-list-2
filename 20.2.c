
/* Program: Open the named FIFO myfifo, read a message sent by
   the writer process, display it, and close the FIFO. */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

int main(void) {
    const char *fifo = "myfifo";
    char buffer[200];
    struct stat st;

    if (stat(fifo, &st) == -1) {
        if (mkfifo(fifo, 0666) == -1) {
            perror("mkfifo");
            return EXIT_FAILURE;
        }
    } else if (!S_ISFIFO(st.st_mode)) {
        fprintf(stderr, "%s is not a FIFO.\n", fifo);
        return EXIT_FAILURE;
    }

    int fd = open(fifo, O_RDONLY);
    if (fd == -1) {
        perror("open");
        return EXIT_FAILURE;
    }

    ssize_t n = read(fd, buffer, sizeof(buffer) - 1);

    if (n == -1) {
        perror("read");
        close(fd);
        return EXIT_FAILURE;
    }

    buffer[n] = '\0';
    printf("Received message: %s\n", buffer);

    close(fd);
    return EXIT_SUCCESS;
}
