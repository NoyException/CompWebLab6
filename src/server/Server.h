//
// Created by Noy on 2023/12/23.
//

#ifndef LAB6SOCKET_SERVER_H
#define LAB6SOCKET_SERVER_H

#include <list>
#include <unordered_map>
#include <thread>
#include <queue>
#include <functional>

#include "ServerSocket.h"

class Server {
private:
    class SocketHandler {
    private:
        Server *server_;
        std::unique_ptr<Socket> socket_;
        std::thread receiver_;
        std::thread sender_;
        std::string id_;
        std::queue<std::function<void(Socket&)>> messages_;
        bool running_ = true;
        long long timestamp_ = 0;
        void receive();
    public:
        SocketHandler(Server* server, std::unique_ptr<Socket> socket);
        ~SocketHandler();
        void disconnect();
        void send(const std::function<void(Socket &)> &message);
        [[nodiscard]] bool isConnected() const;
        [[nodiscard]] bool isTimeout() const;
        [[nodiscard]] const std::string& getId() const;
    };

    ServerSocket serverSocket_;
    std::list<std::unique_ptr<SocketHandler>> socketHandlers_;
    std::unordered_map<std::string, SocketHandler*> idToSocketHandler_;
    std::thread *heartbeat_ = nullptr;
    bool running_ = true;

public:
    explicit Server(int port);
    ~Server();
    void start();
    void stop();
};


#endif //LAB6SOCKET_SERVER_H
