//
// Created by Noy on 2023/12/21.
//

#ifndef LAB6SOCKET_ADDRESS_H
#define LAB6SOCKET_ADDRESS_H

#ifdef __linux__
#include <sys/socket.h>
#include <arpa/inet.h>
#endif
#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#endif
#include <string>

class Address {
private:
    friend class Socket;
    friend class ServerSocket;
    sockaddr_in address_;
    std::string ip_;
    int port_;
public:
    Address(const std::string& ip, int port);
    [[nodiscard]] const std::string& getIp() const;
    [[nodiscard]] int getPort() const;
    [[nodiscard]] std::string toString() const;

    bool operator==(const Address& another) const;
    bool operator!=(const Address& another) const;


};


#endif //LAB6SOCKET_ADDRESS_H
