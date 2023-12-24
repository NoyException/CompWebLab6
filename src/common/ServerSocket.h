//
// Created by Noy on 2023/12/21.
//

#ifndef LAB6SOCKET_SERVERSOCKET_H
#define LAB6SOCKET_SERVERSOCKET_H

#include <memory>

#include "Socket.h"

class ServerSocket {
private:
    int fd_;
    int port_;
public:
    ServerSocket();
    void bind(const Address& address);
    void bind(const std::string& ip, int port);
    void bind(int port);
    void listen();
    std::unique_ptr<Socket> accept();

    void close();
    [[nodiscard]] bool isClosed() const;
    [[nodiscard]] int getPort() const;
};


#endif //LAB6SOCKET_SERVERSOCKET_H
