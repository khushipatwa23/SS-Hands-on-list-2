
/* Program: Wait for data to arrive in the named FIFO myfifo for
   a maximum of 10 seconds using select(), then display the data
   received or report that the timeout expired. */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/select.h>

int main(void) {
    const char *fifo = "myfifo";
    char buffer[200];
    struct stat st;

    if (mkfifo(fifo, 0666) == -1) {
        if (stat(fifo, &st) == -1 || !S_ISFIFO(st.st_mode)) {
            perror("mkfifo/stat");
            return EXIT_FAILURE;
        }
    }

    // Linux permits opening a FIFO read/write without another process.
    // This avoids immediate EOF while waiting for an external writer.
    int fd = open(fifo, O_RDWR | O_NONBLOCK);

    if (fd == -1) {
        perror("open");
        return EXIT_FAILURE;
    }

    fd_set readfds;
    struct timeval timeout;

    FD_ZERO(&readfds);
    FD_SET(fd, &readfds);

    timeout.tv_sec = 10;
    timeout.tv_usec = 0;

    printf("Waiting for FIFO data for 10 seconds...\n");
    fflush(stdout);

    int result = select(fd + 1, &readfds, NULL, NULL, &timeout);

    if (result == -1) {
        perror("select");
        close(fd);
        return EXIT_FAILURE;
    }

    if (result == 0) {
        printf("Timeout: No data received within 10 seconds.\n");
        close(fd);
        return EXIT_SUCCESS;
    }

    if (FD_ISSET(fd, &readfds)) {
        ssize_t n = read(fd, buffer, sizeof(buffer) - 1);

        if (n == -1) {
            perror("read");
            close(fd);
            return EXIT_FAILURE;
        }

        if (n == 0) {
            printf("No data available.\n");
        } else {
            buffer[n] = '\0';
            printf("Received data: %s\n", buffer);
        }
    }

    close(fd);
    return EXIT_SUCCESS;
}
