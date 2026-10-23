
/* Program: TCP server for communication between two machines. */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define PORT 5000

int main(void) {
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1) {
        perror("socket");
        return 1;
    }

    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

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

    if (listen(server_fd, 5) == -1) {
        perror("listen");
        close(server_fd);
        return 1;
    }

    printf("Server listening on port %d...\n", PORT);

    int client_fd = accept(server_fd, NULL, NULL);
    if (client_fd == -1) {
        perror("accept");
        close(server_fd);
        return 1;
    }

    char buffer[1024];
    ssize_t n;

    while ((n = recv(client_fd, buffer, sizeof(buffer) - 1, 0)) > 0) {
        buffer[n] = '\0';
        printf("Client: %s", buffer);
        fflush(stdout);
    }

    if (n == -1)
        perror("recv");

    const char *reply = "Hello from server\n";
    size_t sent = 0;
    size_t len = strlen(reply);

    while (sent < len) {
        ssize_t k = send(client_fd, reply + sent, len - sent,
                         MSG_NOSIGNAL);
        if (k == -1) {
            if (errno == EINTR)
                continue;
            perror("send");
            break;
        }
        sent += (size_t)k;
    }

    close(client_fd);
    close(server_fd);
    return 0;
}
