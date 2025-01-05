#include <iostream>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define IP "127.0.0.1"
#define PORT 8080

int main() {
    // Prompt the client to enter username
    // std::string username;
    // std::cout << "Enter your username: ";
    // std::getline(std::cin, username);

    int sockfd;
    struct sockaddr_in server_addr;
    int recv_length = 1;
    char buffer[1024];

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    inet_aton(IP, &server_addr.sin_addr);
    memset(&server_addr.sin_zero, '\0', 8);

    if ((sockfd = socket(PF_INET, SOCK_STREAM, 0)) == -1)
        perror("Could not create socket");

    if (connect(sockfd, (struct sockaddr*)&server_addr, sizeof(struct sockaddr_in)) == -1)
        perror("Could not connect to server");

    printf("Connected to server at %s:%d\n", IP, PORT);

    std::string hello = "Hello from client\n";
    send(sockfd, hello.data(), hello.length(), 0);
    printf("Hello message sent\n");

    recv_length = recv(sockfd, buffer, sizeof(buffer) - 1, 0);
    while (recv_length > 0) {
        buffer[recv_length] = '\0'; // Null-terminate the buffer
        printf("RECV (%d bytes): %s\n", recv_length, buffer);
        recv_length = recv(sockfd, buffer, sizeof(buffer) - 1, 0);
    }

    close(sockfd);

    return 0;
}
