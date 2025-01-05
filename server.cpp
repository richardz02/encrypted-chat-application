#include <cstdio>
#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define PORT 8080

int main() {
    int sockfd, new_sockfd;
    struct sockaddr_in host_addr, client_addr;
    socklen_t sin_size;
    int recv_length = 1, yes = 1;
    char buffer[1024];

    // Create a socket
    if ((sockfd = socket(PF_INET, SOCK_STREAM, 0)) == -1)
        perror("Could not create socket");

    // Set SO_READDR socket option to true
    // For reusing a given address for binding
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1)
        perror("Could not set socket options");

    // Set up the host address structure
    host_addr.sin_family = AF_INET;
    host_addr.sin_port = htons(PORT);
    host_addr.sin_addr.s_addr = INADDR_ANY;
    memset(&(host_addr.sin_zero), '\0', 8);

    // Bind the socket to the host address
    if (bind(sockfd, (struct sockaddr*)&host_addr, sizeof(struct sockaddr)) == -1)
        perror("Could not bind socket to address");

    // Listen for connections on socketfd
    if (listen(sockfd, 5) == -1)
        perror("Could not listen on socket");

    printf("Server is active on port %d\n", ntohs(host_addr.sin_port));

    while (1) {
        sin_size = sizeof(struct sockaddr_in);

        // New socket is created after connection is accepted
        new_sockfd = accept(sockfd, (struct sockaddr*)&client_addr, &sin_size);

        if (new_sockfd == -1)
            perror("Could not accept connection");

        printf("Server: got connection from %s on port %d\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

        // Send a message back to the client
        const char* message = "Hello from server\n";
        send(new_sockfd, message, strlen(message), 0);

        recv_length = recv(new_sockfd, buffer, sizeof(buffer) - 1, 0);
        while (recv_length > 0) {
            buffer[recv_length] = '\0'; // Null-terminate the buffer
            printf("RECV (%d bytes): %s\n", recv_length, buffer);
            recv_length = recv(new_sockfd, buffer, sizeof(buffer) - 1, 0);
        }

        // Close socket after communication
        close(new_sockfd);
    }

    close(sockfd);

    return 0;
}
