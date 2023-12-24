//
// Created by Noy on 2023/12/21.
//

#include "Address.h"

#include <stdexcept>

Address::Address(const std::string &ip, int port) : ip_(ip=="localhost"?"127.0.0.1":ip), port_(port) {
    address_.sin_family = AF_INET;
    address_.sin_port = htons(port);
    address_.sin_addr.s_addr = inet_addr(ip_.c_str());
}

Address::Address(const sockaddr &address) {
    address_ = *((sockaddr_in*)&address);
    ip_ = inet_ntoa(address_.sin_addr);
    port_ = ntohs(address_.sin_port);
}

const std::string &Address::getIp() const {
    return ip_;
}

int Address::getPort() const {
    return port_;
}

std::string Address::toString() const {
    return ip_ + ":" + std::to_string(port_);
}

bool Address::operator==(const Address &another) const {
    return ip_ == another.ip_ && port_ == another.port_;
}

bool Address::operator!=(const Address &another) const {
    return !(*this == another);
}

std::ostream &operator<<(std::ostream &os, const Address &address) {
    os << address.toString();
    return os;
}