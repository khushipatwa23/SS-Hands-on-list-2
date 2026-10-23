
/* Program: Communicate bidirectionally with Program B using two
   named FIFOs. Program A sends a message through fifo_ab and
   receives B's reply through fifo_ba. */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

static void create_fifo(const char *name) {
    struct stat st;

    if (mkfifo(name, 0666) == -1) {
        if (stat(name, &st) == -1 || !S_ISFIFO(st.st_mode)) {
            perror(name);
            exit(EXIT_FAILURE);
        }
    }
}

int main(void) {
    char buffer[200];

    create_fifo("fifo_ab");
    create_fifo("fifo_ba");

    int out = open("fifo_ab", O_WRONLY);
    if (out == -1) {
        perror("open fifo_ab");
        return EXIT_FAILURE;
    }

    int in = open("fifo_ba", O_RDONLY);
    if (in == -1) {
        perror("open fifo_ba");
        close(out);
        return EXIT_FAILURE;
    }

    printf("Program A - Enter message: ");
    fflush(stdout);

    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        close(out);
        close(in);
        return EXIT_FAILURE;
    }

    size_t len = strlen(buffer);

    if (write(out, buffer, len) == -1) {
        perror("write");
        close(out);
        close(in);
        return EXIT_FAILURE;
    }

    ssize_t n = read(in, buffer, sizeof(buffer) - 1);

    if (n == -1) {
        perror("read");
        close(out);
        close(in);
        return EXIT_FAILURE;
    }

    if (n == 0) {
        printf("No reply received.\n");
    } else {
        buffer[n] = '\0';
        printf("Reply from B: %s\n", buffer);
    }

    close(out);
    close(in);

    return EXIT_SUCCESS;
}
