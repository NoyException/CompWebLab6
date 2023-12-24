//
// Created by Noy on 2023/12/20.
//
#include <iostream>

#include "TCPSocket.h"
#include "CommandExecutor.h"
#include "MessageType.h"
#include "SocketHandler.h"

bool running = true;

int main() {

#ifdef _WIN32
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);
#endif

    CommandExecutor executor;
    auto* handler = new SocketHandler();

    executor.registerCommand("exit", [&](std::string args[]){
        handler->close();
        running = false;
    });

    executor.registerCommand("connect", [&](std::string args[]){
        std::string ip = args[1];
        int port = std::stoi(args[2]);
        std::string id = args[3];
        if(handler->isConnected()){
            handler->close();
            delete handler;
            handler = new SocketHandler();
        }
        handler->connect(Address(ip, port), id);
    });

    executor.registerCommand("send", [&](std::string args[]){
        if(!handler->isConnected()){
            std::cout << "Not connected" << std::endl;
            return;
        }
        std::string dest = args[1];
        std::string message = args[2];
        handler->send([=](Socket& socket){
            auto type = static_cast<char>(MessageType::SEND_MESSAGE);
            std::cout << "*To " << dest << ": " << message << std::endl;
            socket.writeChar(type);
            socket.writeString(dest);
            socket.writeString(message);
        });
    });

    executor.registerCommand("get", [&](std::string args[]){
        std::string name = args[1];
        std::string value = handler->get(name);
        std::cout << "[Server]: " << value << std::endl;
    });

    while(running){
        std::string command;
        std::getline(std::cin, command);
        executor.execute(command);
    }

#ifdef _WIN32
    WSACleanup();
#endif
    return 0;
}
