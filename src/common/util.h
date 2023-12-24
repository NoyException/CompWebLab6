//
// Created by Noy on 2023/12/23.
//

#ifndef LAB6SOCKET_UTIL_H
#define LAB6SOCKET_UTIL_H

#include <string>
#include <memory>
#include <vector>

std::string format(const char *fmt, ...);

std::unique_ptr<std::vector<std::string>> split(const std::string &str, const std::string &delim);

long long currentTimeMillis();

#endif //LAB6SOCKET_UTIL_H
