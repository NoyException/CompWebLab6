//
// Created by Noy on 2023/12/21.
//

#include "Socket.h"

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

Socket::Socket() : Socket(-1) {}

Socket::Socket(int fd) : fd_(fd) {
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

Socket::Socket(const Socket &another) = default;

Socket &Socket::operator=(const Socket &another) = default;

Socket::Socket(Socket &&another) noexcept : fd_(another.fd_) {
    another.fd_ = -1;
}

Socket &Socket::operator=(Socket &&another) noexcept {
    fd_ = another.fd_;
    another.fd_ = -1;
    return *this;
}

Socket::~Socket() {
    close();
}

void Socket::close() {
    if (fd_ != -1) {
        ::close(fd_);
        fd_ = -1;
    }
}

int Socket::getFd() const {
    return fd_;
}

bool Socket::isConnected() const {
    int error = 0;
    socklen_t len = sizeof (error);
    int retval = getsockopt (fd_, SOL_SOCKET, SO_ERROR, (char*)&error, &len);

    if (retval != 0) {
        /* there was a problem getting the error code */
        throw std::runtime_error("getsockopt error");
    }

    if (error != 0) {
        /* socket has a non zero error status */
        return false;
    }

    return true;
}

void Socket::connect(const Address &address) {
    if(::connect(fd_, (sockaddr*)&address.address_, sizeof(address.address_))<0){
#ifdef _WIN32
        int a = WSAGetLastError();
        printf("connect function failed with error = %d\n", a);
        WSACleanup();
#else
        perror("connect error");
#endif
        throw std::runtime_error("connect error");
    }
    address_ = address;
}

void Socket::connect(const std::string& ip, int port) {
    connect({ip, port});
}

std::optional<Address> Socket::getAddress() const {
    return address_;
}

bool Socket::isClosed() const {
    return fd_ == -1;
}

bool Socket::isReadable() const {
    fd_set fds;
    struct timeval tv{};

    FD_ZERO(&fds);
    FD_SET(fd_, &fds);

    tv.tv_sec = 0;
    tv.tv_usec = 0;

    int ret = select(fd_ + 1, &fds, NULL, NULL, &tv);
    if (ret == -1) {
        throw std::runtime_error("select error");
    }

    return FD_ISSET(fd_, &fds);
}

bool Socket::isWritable() const {
    fd_set fds;
    struct timeval tv{};

    FD_ZERO(&fds);
    FD_SET(fd_, &fds);

    tv.tv_sec = 0;
    tv.tv_usec = 0;

    int ret = select(fd_ + 1, NULL, &fds, NULL, &tv);
    if (ret == -1) {
        throw std::runtime_error("select error");
    }

    return FD_ISSET(fd_, &fds);
}

void Socket::read(char *buf, size_t len) {
    ::recv(fd_, buf, len, 0);
}

char Socket::readChar() {
    char buf;
    read(&buf, 1);
    return buf;
}

int Socket::readInt() {
    int buf;
    read((char *) &buf, 4);
    return buf;
}

std::string Socket::readString() {
    short len;
    read((char *) &len, 2);
    char buf[len];
    read(buf, len);
    return {buf, (size_t)len};
}

void Socket::readAll(char *buf) {
    ssize_t len = 0;
    while (true) {
        ssize_t ret = ::read(fd_, buf + len, 1024);
        if (ret == 0) {
            break;
        } else if (ret == -1) {
            throw std::runtime_error("read error");
        }
        len += ret;
    }
}

void Socket::write(const char *buf, size_t len) {
    ::send(fd_, buf, len, 0);
}

void Socket::write(const std::string &str) {
    write(str.c_str(), str.length());
}

void Socket::writeChar(char c) {
    write(&c, 1);
}

void Socket::writeInt(int i) {
    write((char *) &i, 4);
}

void Socket::writeString(const std::string &str) {
    auto len = (short)str.length();
    write((char *) &len, 2);
    write(str.c_str(), len);
}

void Socket::flush() {
    char buf;
    while (isReadable()) {
        read(&buf, 1);
    }
}

void Socket::setTimeout(long ms) {
    struct timeval tv{};
    tv.tv_sec = ms / 1000;
    tv.tv_usec = (ms % 1000) * 1000;
    setsockopt(fd_, SOL_SOCKET, SO_RCVTIMEO, (const char *) &tv, sizeof(tv));
}

bool Socket::isTimeout() const {
    fd_set fds;
    struct timeval tv{};

    FD_ZERO(&fds);
    FD_SET(fd_, &fds);

    tv.tv_sec = 1;
    tv.tv_usec = 0;

    int ret = select(fd_ + 1, &fds, NULL, NULL, &tv);
    if (ret == -1) {
        throw std::runtime_error("select error");
    }

    return ret == 0;
}
