#include <cstdlib>
#include <iostream>
#include <thread>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <string>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define IP "127.0.0.1"
#define PORT 8080

void receive_message(int server_fd) {
    char buffer[1024];
    while (1) {
        memset(buffer, 0, sizeof(buffer));
        int buffer_length = recv(server_fd, buffer, sizeof(buffer) - 1, 0);
        if (buffer_length <= 0) {
            std::cout << "Disconnected from server." << std::endl;
            exit(1);
        }
        printf("Message: %s\n", buffer);
    }
}

int main() {
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

    if (connect(sockfd, (struct sockaddr*)&server_addr, sizeof(struct sockaddr_in)) == -1) {
        perror("Could not connect to server");
        exit(1);
    }

    printf("Connected to server at %s:%d\n", IP, PORT);

    std::thread(receive_message, sockfd).detach();

    std::string message;
    while (1) {
        std::getline(std::cin, message);
        send(sockfd, message.c_str(), message.length(), 0);
    }

    close(sockfd);

    return 0;
}
