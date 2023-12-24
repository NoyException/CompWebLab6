//
// Created by Noy on 2023/12/21.
//

#include "ServerSocket.h"
#include "Address.h"

#ifdef __linux__
#include <sys/socket.h>
#include <arpa/inet.h>
#include <pcap/socket.h>
#endif
#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#endif

#include <unistd.h>
#include <stdexcept>

ServerSocket::ServerSocket() {
    fd_ = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (fd_ == INVALID_SOCKET) {
#ifdef _WIN32
        int a = WSAGetLastError();
        printf("socket function failed with error = %d\n", a);
        WSACleanup();
#else
        perror("socket error");
#endif
        throw std::runtime_error("socket error");
    }
}

void ServerSocket::bind(const Address& address) {
    if (::bind(fd_, (sockaddr*)&address.address_, sizeof(address.address_)) < 0) {
        throw std::runtime_error("bind error");
    }
    port_ = address.port_;
}

void ServerSocket::bind(const std::string& ip, int port) {
    bind({ip, port});
}

void ServerSocket::bind(int port) {
    sockaddr_in serv_addr{};
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    if (::bind(fd_, (sockaddr*)&serv_addr, sizeof(serv_addr)) == SOCKET_ERROR) {
        perror("bind error");
        throw std::runtime_error("bind error");
    }
    port_ = port;
}

void ServerSocket::listen() {
    if (::listen(fd_, SOMAXCONN) < 0) {
        perror("listen error");
        throw std::runtime_error("listen error");
    }
}

std::unique_ptr<Socket> ServerSocket::accept() {
    sockaddr t{};
    socklen_t len = sizeof(sockaddr);
    int client_fd = ::accept(fd_, &t, &len);
    if (client_fd < 0) {
        perror("accept error");
        throw std::runtime_error("accept error");
    }
    auto socket = std::make_unique<Socket>(client_fd);
    socket->address_ = Address(t);
    return socket;
}

void ServerSocket::close() {
    if (fd_ != -1) {
        ::close(fd_);
        fd_ = -1;
    }
}

bool ServerSocket::isClosed() const {
    return fd_ == -1;
}

int ServerSocket::getPort() const {
    return port_;
}