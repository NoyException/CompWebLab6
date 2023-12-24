//
// Created by Noy on 2023/12/20.
//
#include <iostream>
#include <thread>

#include "Server.h"

int main() {

#ifdef _WIN32
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);
#endif

    Server server(5713);
    server.start();

#ifdef _WIN32
    WSACleanup();
#endif
    return 0;
}
