#include <cstdio>
#include <iostream>
#include <thread>
#include <unistd.h>
#include <unordered_map>
#include <stdlib.h>
#include <bits/stdc++.h>
#include <string.h>
#include <vector>
#include <mutex>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <nlohmann/json.hpp>

#define PORT 8080

std::vector<int> clients;
std::unordered_map<std::string, int> map;
std::mutex client_mutex;

void send_message(const std::string& message, int sender_sockfd) {
    std::lock_guard<std::mutex> lock(client_mutex);
    for (int client_fd : clients) {
        if (client_fd != sender_sockfd)
            send(client_fd, message.c_str(), message.length(), 0);
    }
}

void handle_client(int client_sockfd) {
    char buffer[1024];

    while (1) {
        memset(buffer, 0, sizeof(buffer));
        int buffer_length = recv(client_sockfd, buffer, sizeof(buffer) - 1, 0);

        if (buffer_length <= 0) {
            std::cout << "User disconnected." << std::endl;
            std::lock_guard<std::mutex> lock(client_mutex);
            clients.erase(std::find(clients.begin(), clients.end(), client_sockfd));
            close(client_sockfd);
            break;
        }

        nlohmann::json j = nlohmann::json::parse(buffer);
        std::string message = j.at("payload");
        std::cout << "Payload is " << message << std::endl;

        printf("RECV (%d bytes): %s\n", (int)message.length(), message.c_str());
        send_message(message, client_sockfd);
    }
}

int main() {
    int server_sockfd, new_sockfd;
    struct sockaddr_in host_addr;
    socklen_t sin_size;
    int recv_length = 1, yes = 1;
    char buffer[1024];

    // Create a socket
    if ((server_sockfd = socket(PF_INET, SOCK_STREAM, 0)) == -1)
        perror("Could not create socket");

    // Set SO_READDR socket option to true to enable reusing a given address for binding
    if (setsockopt(server_sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1)
        perror("Could not set socket options");

    // Set up the host address structure
    host_addr.sin_family = AF_INET;
    host_addr.sin_port = htons(PORT);
    host_addr.sin_addr.s_addr = INADDR_ANY;
    memset(&(host_addr.sin_zero), '\0', 8);

    // Bind the socket to the host address
    if (bind(server_sockfd, (struct sockaddr*)&host_addr, sizeof(struct sockaddr)) == -1)
        perror("Could not bind socket to address");

    // Listen for connections on socketfd
    if (listen(server_sockfd, 5) == -1)
        perror("Could not listen on socket");

    printf("Server is active on port %d\n", ntohs(host_addr.sin_port));

    while (1) {
        struct sockaddr_in client_addr;
        sin_size = sizeof(struct sockaddr_in);

        // New socket is created after connection is accepted
        new_sockfd = accept(server_sockfd, (struct sockaddr*)&client_addr, &sin_size);

        if (new_sockfd == -1)
            perror("Could not accept connection");

        std::lock_guard<std::mutex> lock(client_mutex);
        clients.push_back(new_sockfd);

        printf("Server: got connection from %s on port %d\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

        // Apply thread
        std::thread(handle_client, new_sockfd).detach();
    }

    close(server_sockfd);

    return 0;
}
