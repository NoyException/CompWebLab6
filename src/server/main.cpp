//
// Created by Noy on 2023/12/20.
//
#include <iostream>
#include "ServerSocket.h"

int main() {

#ifdef _WIN32
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);
#endif

    ServerSocket serverSocket;
    serverSocket.bind(8080);
    serverSocket.listen();
    std::cout << "start listening!" << std::endl;

    while (true){
        Socket socket = serverSocket.accept();
        std::cout << "accept" << std::endl;
        socket.writeUTF("hello world from server");
        std::string s = socket.readUTF();
        std::cout << s << std::endl;
        socket.close();
    }

#ifdef _WIN32
    WSACleanup();
#endif
    return 0;
}
