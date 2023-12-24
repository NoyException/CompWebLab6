//
// Created by Noy on 2023/12/23.
//

#ifndef LAB6SOCKET_COMMANDEXECUTOR_H
#define LAB6SOCKET_COMMANDEXECUTOR_H

#include <unordered_map>
#include <functional>
#include <string>

class CommandExecutor {
private:
    std::unordered_map<std::string, std::function<void(std::string[])>> commands_;
public:
    CommandExecutor();
    void execute(const std::string& command);
    void execute(std::string args[]);
    void registerCommand(const std::string &command, std::function<void(std::string[])> func);
};


#endif //LAB6SOCKET_COMMANDEXECUTOR_H
