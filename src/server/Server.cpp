//
// Created by Noy on 2023/12/23.
//

#include "Server.h"
#include "MessageType.h"
#include "util.h"

#include <iostream>
#include <sstream>
#include <iomanip>

Server::Server(int port) : serverSocket_() {
    serverSocket_.bind(port);
}

Server::~Server() {
    stop();
}

void Server::start() {
    serverSocket_.listen();
    std::cout << "start listening at port " << serverSocket_.getPort() << std::endl;

    heartbeat_ = new std::thread([&] {
        while (running_) {
            for (auto it = socketHandlers_.begin(); it != socketHandlers_.end();) {
                auto handler = it->get();
                if (!handler->isConnected()) {
                    idToSocketHandler_.erase(handler->getId());
                    it = socketHandlers_.erase(it);
                } else if (handler->isTimeout()) {
                    std::cout << handler->getId()
                              << " timeout"
                              << std::endl;
                    handler->disconnect();
                    idToSocketHandler_.erase(handler->getId());
                    it = socketHandlers_.erase(it);
                } else {
                    ++it;
                }
            }
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    });

    while (running_) {
        std::unique_ptr<Socket> socket = serverSocket_.accept();
        std::unique_ptr<SocketHandler> handler = std::make_unique<SocketHandler>(this, std::move(socket));
        socketHandlers_.push_back(std::move(handler));
    }
}

void Server::stop() {
    running_ = false;
    if (heartbeat_ != nullptr)
        heartbeat_->join();
    for (auto &handler: socketHandlers_) {
        handler->disconnect();
    }
}

Server::SocketHandler::SocketHandler(Server *server, std::unique_ptr<Socket> socket) :
        server_(server),
        socket_(std::move(socket)),
        timestamp_(-1),
        thread_([this]() {
            while (running_) {
                receive();
            }
        }) {}

void Server::SocketHandler::receive() {
    char c = socket_->readChar();
    auto type = static_cast<MessageType>(c);
    switch (type) {
        case MessageType::SEND_MESSAGE: {
            std::string dest = socket_->readString();
            std::string message = socket_->readString();

            std::cout << id_
                      << " send to "
                      << dest
                      << ": "
                      << message
                      << std::endl;

            auto it = server_->idToSocketHandler_.find(dest);
            if (it != server_->idToSocketHandler_.end()) {
                it->second->socket_->writeChar(static_cast<char>(MessageType::SEND_MESSAGE));
                it->second->socket_->writeString(id_);
                it->second->socket_->writeString(message);
            } else {
                socket_->writeChar(static_cast<char>(MessageType::SEND_MESSAGE));
                socket_->writeString("[Server]");
                socket_->writeString("User not found");
            }
            break;
        }
        case MessageType::GET: {
            int requestId = socket_->readInt();
            std::string s = socket_->readString();

            std::cout << id_
                      << " request "
                      << s
                      << std::endl;

            socket_->writeChar(static_cast<char>(MessageType::GET));
            socket_->writeInt(requestId);

            if (s == "users") {
                std::stringstream ss;
                ss << "there are " << server_->idToSocketHandler_.size() << " users:";
                for (auto &it: server_->idToSocketHandler_) {
                    ss << std::endl << it.first << " from " << *it.second->socket_->getAddress();
                }
                socket_->writeString(ss.str());
            }
            else if (s == "time") {
                auto now = std::chrono::system_clock::now();
                auto time = std::chrono::system_clock::to_time_t(now);
                std::tm *tm = std::localtime(&time);
                std::stringstream ss;
                ss << std::put_time(tm, "%Y-%m-%d %H:%M:%S");
                socket_->writeString(ss.str());
            }
            else if (s == "name") {
                char computerName[MAX_COMPUTERNAME_LENGTH + 1];
                DWORD size = sizeof(computerName) / sizeof(computerName[0]);

                if (GetComputerName(computerName, &size)) {
                    socket_->writeString(std::string(computerName));
                } else {
                    std::cerr << "Failed to get computer name." << std::endl;
                }
            } else {
                socket_->writeString("Unknown request");
            }
            break;
        }
        case MessageType::CONNECT:
            timestamp_ = currentTimeMillis();
            id_ = socket_->readString();
            if (server_->idToSocketHandler_.find(id_) != server_->idToSocketHandler_.end()) {
                id_ = "";
                socket_->writeChar(static_cast<char>(MessageType::SEND_MESSAGE));
                socket_->writeString("[Server]");
                socket_->writeString("User already exists");
                break;
            }

            server_->idToSocketHandler_[id_] = this;
            std::cout << id_
                      << " is connected"
                      << std::endl;
            break;
        case MessageType::DISCONNECT:
            std::cout << id_
                      << " disconnect"
                      << std::endl;
            running_ = false;
            socket_->writeChar(static_cast<char>(MessageType::DISCONNECT));
            socket_->close();
            break;
        case MessageType::HEARTBEAT:
            timestamp_ = currentTimeMillis();
            break;
        default:
            break;
    }
}

void Server::SocketHandler::disconnect() {
    running_ = false;
    socket_->writeChar(static_cast<char>(MessageType::DISCONNECT));
    socket_->close();
    if (thread_.joinable())
        thread_.join();
}

Server::SocketHandler::~SocketHandler() {
    running_ = false;
    if (thread_.joinable())
        thread_.join();
}

bool Server::SocketHandler::isTimeout() const {
    if (!isConnected()) return false;
    long long now = currentTimeMillis();
    return timestamp_ + 5000 < now;
}

bool Server::SocketHandler::isConnected() const {
    return running_ && !id_.empty();
}

const std::string &Server::SocketHandler::getId() const {
    return id_;
}

