#pragma once

#include <sys/types.h>
#include <sys/stat.h>
#include <cstdio>
#include <string>
#include <iostream>
#include <vector>

#include <openZia/OperatingSystem.hpp>

#if defined(SYSTEM_WINDOWS)
#define popen _popen
#define pclose _pclose
#elif defined(SYSTEM_LINUX)

#include <unistd.h>

#endif

class ExeCommand {
public:
    enum Type {
        READ,
        WRITE,
    };

    explicit ExeCommand(uint16_t size = 10);
    explicit ExeCommand(std::string command, ExeCommand::Type type = READ, uint16_t size = 10);
    ~ExeCommand() = default;

    void reConf(const std::string &command, ExeCommand::Type type = READ) noexcept;
    [[nodiscard]] std::string exec() const;

private:
    std::string _command;
    ExeCommand::Type _type;
    uint16_t _readSize;
};
