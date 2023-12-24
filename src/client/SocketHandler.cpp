//
// Created by Noy on 2023/12/24.
//

#include "SocketHandler.h"
#include "MessageType.h"

#include <stdexcept>
#include <iostream>

SocketHandler::SocketHandler() : socket_(), connected_(false) {
}

SocketHandler::~SocketHandler() {
    delete receiver_;
    delete sender_;
    delete heartbeat_;
    for (auto &item: requests_){
        delete item.second;
    }
}

bool SocketHandler::isConnected() const {
    return connected_;
}

void SocketHandler::connect(const Address &address, const std::string &id) {
    if (connected_) {
        throw std::runtime_error("Already connected");
    }
    socket_.connect(address);
    id_ = id;
    connected_ = true;

    receiver_ = new std::thread([&]() {
        while (connected_) {
            receive();
        }
    });

    sender_ = new std::thread([&]() {
        while (connected_) {
            while (!messages_.empty()) {
                messages_.front()(socket_);
                messages_.pop();
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
    });

    heartbeat_ = new std::thread([&]() {
        while (connected_) {
            std::this_thread::sleep_for(std::chrono::seconds(1));
            send([&](Socket &) {
                auto type = static_cast<char>(MessageType::HEARTBEAT);
                socket_.writeChar(type);
            });
        }
    });

    send([this, address](Socket &socket) {
        std::cout << "connect to " << address << " as " << id_ << std::endl;
        auto type = static_cast<char>(MessageType::CONNECT);
        socket.writeChar(type);
        socket.writeString(id_);
    });
}

void SocketHandler::send(const std::function<void(Socket &)> &message) {
    messages_.emplace(message);
}

std::string SocketHandler::get(const std::string &name) {
    std::string result;
    int requestId = nextRequestId_++;
    send([=](Socket &socket) {
        auto type = static_cast<char>(MessageType::GET);
        socket.writeChar(type);
        socket.writeInt(requestId);
        socket.writeString(name);
    });
    auto value = new std::promise<std::string>();
    requests_[requestId] = value;
    return value->get_future().get();
}

void SocketHandler::close(bool notifyServer) {
    if (!connected_) {
        return;
    }
    connected_ = false;
    closeThread(heartbeat_);
    closeThread(sender_);
    if (notifyServer) {
        auto type = static_cast<char>(MessageType::DISCONNECT);
        socket_.writeChar(type);
    }
    closeThread(receiver_);
    socket_.close();
}

void SocketHandler::close() {
    close(true);
}

void SocketHandler::closeThread(std::thread *&thread) {
    if (thread != nullptr) {
        if (thread->joinable())
            thread->join();
        delete thread;
        thread = nullptr;
    }
}

void SocketHandler::receive() {
    char c = socket_.readChar();
    auto type = static_cast<MessageType>(c);
    switch (type) {
        case MessageType::SEND_MESSAGE:
            std::cout << socket_.readString() << ": " << socket_.readString() << std::endl;
            break;
        case MessageType::DISCONNECT:
            if (connected_) {
                std::cout << "disconnect" << std::endl;
                close(false);
            }
            break;
        case MessageType::GET: {
            int requestId = socket_.readInt();
            std::string data = socket_.readString();
            auto it = requests_.find(requestId);
            if(it != requests_.end()){
                it->second->set_value(data);
                delete it->second;
                requests_.erase(it);
            }
            break;
        }
        default:
            break;
    }
}
