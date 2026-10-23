
/* Program: Create or reuse a named FIFO called myfifo, accept a
   message from the user, and write that message to the FIFO
   for another process to read. */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

int main(void) {
    const char *fifo = "myfifo";
    char message[200];
    struct stat st;

    if (mkfifo(fifo, 0666) == -1) {
        if (stat(fifo, &st) == -1 || !S_ISFIFO(st.st_mode)) {
            perror("mkfifo/stat");
            return EXIT_FAILURE;
        }
    }

    printf("Enter message: ");
    fflush(stdout);

    if (fgets(message, sizeof(message), stdin) == NULL) {
        fprintf(stderr, "No message entered.\n");
        return EXIT_FAILURE;
    }

    int fd = open(fifo, O_WRONLY);
    if (fd == -1) {
        perror("open");
        return EXIT_FAILURE;
    }

    size_t len = strlen(message);

    if (write(fd, message, len) == -1) {
        perror("write");
        close(fd);
        return EXIT_FAILURE;
    }

    printf("Message sent successfully.\n");
    close(fd);

    return EXIT_SUCCESS;
}

