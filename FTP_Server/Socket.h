#pragma once

#include <string>
#include <stdexcept>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>

class Socket {
    int sockfd;

public:
    Socket() {
        sockfd = socket(AF_INET, SOCK_STREAM, 0);
        if (sockfd < 0)
            throw std::runtime_error("Failed to create socket");
    }

    ~Socket() {
        if (sockfd >= 0)
            close(sockfd);
    }

    void connectTo(const std::string& ip, int port) {
        sockaddr_in addr{};
        addr.sin_family = AF_INET;
        addr.sin_port = htons(port);
        if (inet_pton(AF_INET, ip.c_str(), &addr.sin_addr) <= 0)
            throw std::runtime_error("Invalid IP address");

        if (connect(sockfd, reinterpret_cast<sockaddr*>(&addr), sizeof(addr)) < 0)
            throw std::runtime_error("Failed to connect to server");
    }

    void sendData(const void* data, size_t size) {
        send(sockfd, data, size, 0);
    }

    ssize_t recvData(void* buffer, size_t size) {
        return recv(sockfd, buffer, size, 0);
    }

};
