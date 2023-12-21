//
// Created by Noy on 2023/12/20.
//
#include <iostream>

#include "TCPSocket.h"

int main() {
    fork();
#ifdef _WIN32
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);
#endif

    TCPSocket socket;
    socket.connect("localhost", 8080);
    std::cout << "connect" << std::endl;
    std::string s = socket.readUTF();
    std::cout << s;
    socket.writeUTF("hello world from client");
    socket.close();

#ifdef _WIN32
    WSACleanup();
#endif
    return 0;
}
