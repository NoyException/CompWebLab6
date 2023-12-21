//
// Created by Noy on 2023/12/21.
//

#include "Address.h"

#include <stdexcept>

Address::Address(const std::string &ip, int port) : ip_(ip=="localhost"?"127.0.0.1":ip), port_(port) {
    address_.sin_family = AF_INET;
    address_.sin_port = htons(port);
    address_.sin_addr.S_un.S_addr = inet_addr(ip_.c_str());
}
