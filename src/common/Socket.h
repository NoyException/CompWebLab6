//
// Created by Noy on 2023/12/21.
//

#ifndef LAB6SOCKET_SOCKET_H
#define LAB6SOCKET_SOCKET_H

#include "Address.h"

#include <string>
#include <optional>

class Socket {
private:
    friend class ServerSocket;
    int fd_;
    std::optional<Address> address_;
public:
    Socket();
    explicit Socket(int fd);
    Socket(const Socket& another);
    Socket& operator=(const Socket& another);
    Socket(Socket&& another) noexcept;
    Socket& operator=(Socket&& another) noexcept;
    ~Socket();
    [[nodiscard]] int getFd() const;

    [[nodiscard]] bool isConnected() const;
    void connect(const Address& address);
    void connect(const std::string& ip, int port);
    [[nodiscard]] std::optional<Address> getAddress() const;

    [[nodiscard]] bool isClosed() const;
    void close();

    [[nodiscard]] bool isReadable() const;
    void read(char* buf, size_t len);
    void readAll(char* buf);
    char readChar();
    int readInt();
    std::string readString();

    [[nodiscard]] bool isWritable() const;
    void write(const char* buf, size_t len);
    void write(const std::string& str);
    void writeChar(char c);
    void writeInt(int i);
    void writeString(const std::string& str);

    void flush();

    [[nodiscard]] bool isTimeout() const;
    void setTimeout(long ms);
};


#endif //LAB6SOCKET_SOCKET_H
