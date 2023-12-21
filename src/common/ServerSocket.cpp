//
// Created by Noy on 2023/12/21.
//

#include "ServerSocket.h"

#ifdef __linux__
#include <sys/socket.h>
#include <arpa/inet.h>
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
}

void ServerSocket::bind(const std::string& ip, int port) {
    bind({ip, port});
}

void ServerSocket::bind(int port) {
    sockaddr_in serv_addr{};
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);
    serv_addr.sin_addr.S_un.S_addr = INADDR_ANY;
    if (::bind(fd_, (sockaddr*)&serv_addr, sizeof(serv_addr)) == SOCKET_ERROR) {
        perror("bind error");
        throw std::runtime_error("bind error");
    }
}

void ServerSocket::listen() {
    if (::listen(fd_, SOMAXCONN) < 0) {
        perror("listen error");
        throw std::runtime_error("listen error");
    }
}

Socket ServerSocket::accept() {
    SOCKADDR t;
    int len = sizeof(SOCKADDR);
    int client_fd = ::accept(fd_, &t, &len);
    if (client_fd < 0) {
        perror("accept error");
        throw std::runtime_error("accept error");
    }
    return {client_fd};
}

void ServerSocket::close() {
    if (fd_ != -1) {
        ::close(fd_);
        fd_ = -1;
    }
}

bool ServerSocket::isClosed() const {
    char buf;
    return send(fd_, &buf, 0, 0) == -1;
}
