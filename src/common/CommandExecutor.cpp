//
// Created by Noy on 2023/12/23.
//

#include "CommandExecutor.h"
#include "util.h"

#include <iostream>
#include <utility>

CommandExecutor::CommandExecutor() {
    commands_["help"] = [this](std::string args[]) {
        std::cout << "* Available commands:" << std::endl;
        for (auto &command : commands_) {
            std::cout << command.first << std::endl;
        }
    };
    commands_["exit"] = [](std::string args[]) {
        exit(0);
    };
}

void CommandExecutor::execute(const std::string& command) {
    std::unique_ptr<std::vector<std::string>> args = split(command, " ");
    execute(args->data());
}

void CommandExecutor::execute(std::string args[]) {
    if (commands_.find(args[0]) == commands_.end()) {
        std::cout << "* Command not found: " << args[0] << std::endl;
        return;
    }
    commands_[args[0]](args);
}

void CommandExecutor::registerCommand(const std::string &command, std::function<void(std::string[])> func) {
    commands_[command] = std::move(func);
}