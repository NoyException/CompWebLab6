//
// Created by Noy on 2023/12/24.
//

#ifndef LAB6SOCKET_SOCKETHANDLER_H
#define LAB6SOCKET_SOCKETHANDLER_H

#include "TCPSocket.h"

#include <queue>
#include <functional>
#include <thread>
#include <future>

class SocketHandler {
private:
    std::string id_;
    TCPSocket socket_;
    std::queue<std::function<void(Socket&)>> messages_;
    bool connected_{};
    int nextRequestId_;
    std::unordered_map<int, std::promise<std::string>*> requests_;

    std::thread* receiver_ = nullptr;
    std::thread* sender_ = nullptr;
    std::thread* heartbeat_ = nullptr;

    static void closeThread(std::thread*& thread);
    void receive();
    void close(bool notifyServer);
public:
    SocketHandler();
    ~SocketHandler();
    [[nodiscard]] bool isConnected() const;
    void connect(const Address& address, const std::string& id);
    void send(const std::function<void(Socket&)>&);
    std::string get(const std::string& name);
    void close();
};


#endif //LAB6SOCKET_SOCKETHANDLER_H
