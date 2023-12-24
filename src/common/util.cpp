//
// Created by Noy on 2023/12/23.
//

#include "util.h"

#include <cstring>
#include <cstdarg>
#include <chrono>

std::string format(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    char buffer[strlen(fmt) + 1];
    vsprintf(buffer, fmt, args);
    va_end(args);
    return {buffer};
}

std::unique_ptr<std::vector<std::string>> split(const std::string &str, const std::string &delim) {
    std::unique_ptr<std::vector<std::string>> result = std::make_unique<std::vector<std::string>>();
    size_t last = 0;
    size_t next;
    while ((next = str.find(delim, last)) != std::string::npos) {
        result->push_back(str.substr(last, next - last));
        last = next + 1;
    }
    result->push_back(str.substr(last));
    return result;
}

long long currentTimeMillis() {
    return std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::system_clock::now().time_since_epoch()).count();
}