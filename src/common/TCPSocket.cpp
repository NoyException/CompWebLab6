//
// Created by Noy on 2023/12/21.
//

#include "TCPSocket.h"

#ifdef __linux__
#include <sys/socket.h>
#include <arpa/inet.h>
#endif
#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#endif

TCPSocket::TCPSocket() : Socket(socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) {
}

