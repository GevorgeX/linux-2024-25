#pragma once

#include <string>
#include <stdexcept>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>

class Socket {
    int sockfd;
    bool destructed = true;
    Socket(int sockfd) : sockfd(sockfd) {}
public:
    Socket() {
        sockfd = socket(AF_INET, SOCK_STREAM, 0);
        if (sockfd < 0)
            throw std::runtime_error("Failed to create socket");
    }

    ~Socket() {
        if (sockfd >= 0 && destructed)
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

    void bindTo(const std::string& ip, int port) {
        sockaddr_in serverAddress{};
        serverAddress.sin_family = AF_INET;
        serverAddress.sin_port = htons(8080);
        serverAddress.sin_addr.s_addr = inet_addr("127.0.0.1");

        if (bind(sockfd, reinterpret_cast<sockaddr *>(&serverAddress),
                 sizeof(serverAddress)) < 0)
        {
             throw std::runtime_error("Failed to bind to socket");
        }
    }

    void listen(int n = 5) {
        ::listen(sockfd, n);
    }

    Socket accept() {
        int clientSocket = ::accept(sockfd, nullptr, nullptr);
        if (clientSocket < 0) {
            throw std::runtime_error ( "Failed to accept client\n");
        }

        Socket socket(clientSocket);
        socket.destructed = false;
        return socket;
    }

    ssize_t recvData(void* buffer, size_t size) {
        return recv(sockfd, buffer, size, 0);
    }

};
