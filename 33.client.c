
/* Program: TCP client for communication between two machines. */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>

#define PORT 5000
#define SERVER_IP "192.168.1.100"

int main(void) {
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        perror("socket");
        return 1;
    }

    struct sockaddr_in server;
    memset(&server, 0, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);

    if (inet_pton(AF_INET, SERVER_IP, &server.sin_addr) != 1) {
        fprintf(stderr, "Invalid server IP address.\n");
        close(sockfd);
        return 1;
    }

    if (connect(sockfd, (struct sockaddr *)&server, sizeof(server)) == -1) {
        perror("connect");
        close(sockfd);
        return 1;
    }

    const char *message = "Hello from client\n";
    size_t sent = 0;
    size_t len = strlen(message);

    while (sent < len) {
        ssize_t n = send(sockfd, message + sent, len - sent,
                         MSG_NOSIGNAL);
        if (n == -1) {
            if (errno == EINTR)
                continue;
            perror("send");
            close(sockfd);
            return 1;
        }
        sent += (size_t)n;
    }

    shutdown(sockfd, SHUT_WR);

    char buffer[1024];
    ssize_t n;

    while ((n = recv(sockfd, buffer, sizeof(buffer) - 1, 0)) > 0) {
        buffer[n] = '\0';
        printf("Server: %s", buffer);
    }

    if (n == -1)
        perror("recv");

    close(sockfd);
    return n == -1 ? 1 : 0;
}
