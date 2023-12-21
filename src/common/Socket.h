//
// Created by Noy on 2023/12/21.
//

#ifndef LAB6SOCKET_SOCKET_H
#define LAB6SOCKET_SOCKET_H

#include <string>

#include "Address.h"

class Socket {
private:
    int fd_;
public:
    Socket();
    Socket(int fd);
    Socket(const Socket& another);
    Socket& operator=(const Socket& another);
    Socket(Socket&& another) noexcept;
    Socket& operator=(Socket&& another) noexcept;
    ~Socket();
    [[nodiscard]] int getFd() const;

    [[nodiscard]] bool isConnected() const;
    void connect(const Address& address);
    void connect(const std::string& ip, int port);

    [[nodiscard]] bool isClosed() const;
    void close();

    [[nodiscard]] bool isReadable() const;
    void read(char* buf, size_t len);
    void readAll(char* buf);
    std::string readUTF();

    [[nodiscard]] bool isWritable() const;
    void write(const char* buf, size_t len);
    void write(const std::string& str);
    void writeUTF(const std::string& str);

    void flush();

    [[nodiscard]] bool isTimeout() const;
    void setTimeout(long ms);
};


#endif //LAB6SOCKET_SOCKET_H
