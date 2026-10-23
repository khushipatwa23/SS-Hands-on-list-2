
/* Program: Create a concurrent TCP server using pthread_create(). */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define PORT 5001

void *handle_client(void *arg) {
    int client_fd = *(int *)arg;
    free(arg);

    char buffer[1024];
    ssize_t n;

    while ((n = recv(client_fd, buffer, sizeof(buffer), 0)) > 0) {
        size_t sent = 0;

        while (sent < (size_t)n) {
            ssize_t k = send(client_fd, buffer + sent,
                             (size_t)n - sent, MSG_NOSIGNAL);

            if (k == -1) {
                if (errno == EINTR)
                    continue;
                perror("send");
                close(client_fd);
                return NULL;
            }

            if (k == 0) {
                close(client_fd);
                return NULL;
            }

            sent += (size_t)k;
        }
    }

    if (n == -1)
        perror("recv");

    close(client_fd);
    return NULL;
}

int main(void) {
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1) {
        perror("socket");
        return 1;
    }

    int opt = 1;
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR,
                   &opt, sizeof(opt)) == -1) {
        perror("setsockopt");
        close(server_fd);
        return 1;
    }

    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&addr, sizeof(addr)) == -1) {
        perror("bind");
        close(server_fd);
        return 1;
    }

    if (listen(server_fd, 10) == -1) {
        perror("listen");
        close(server_fd);
        return 1;
    }

    printf("Pthread server listening on port %d...\n", PORT);

    while (1) {
        int client_fd = accept(server_fd, NULL, NULL);

        if (client_fd == -1) {
            if (errno == EINTR)
                continue;
            perror("accept");
            continue;
        }

        int *arg = malloc(sizeof(int));
        if (arg == NULL) {
            perror("malloc");
            close(client_fd);
            continue;
        }

        *arg = client_fd;

        pthread_t tid;
        int rc = pthread_create(&tid, NULL, handle_client, arg);

        if (rc != 0) {
            fprintf(stderr, "pthread_create: %s\n", strerror(rc));
            free(arg);
            close(client_fd);
            continue;
        }

        rc = pthread_detach(tid);
        if (rc != 0)
            fprintf(stderr, "pthread_detach: %s\n", strerror(rc));
    }

    close(server_fd);
    return 0;
}
