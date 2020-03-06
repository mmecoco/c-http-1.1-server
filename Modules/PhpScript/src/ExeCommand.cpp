#include "ExeCommand.hpp"

#include <utility>

ExeCommand::ExeCommand(uint16_t size)
        : _command(""), _type(ExeCommand::READ), _readSize(size)
{}

ExeCommand::ExeCommand(std::string command, ExeCommand::Type type, uint16_t size)
        : _command(std::move(command)), _type(type), _readSize(size)
{}

void ExeCommand::reConf(const std::string &command, ExeCommand::Type type) noexcept
{
    _command = command;
    _type = type;
}

std::string ExeCommand::exec() const
{
    std::string command_output = "";   //FIXED BY HAND (ELIE)
    std::vector<char> buffer(_readSize);
    FILE *output = popen(_command.c_str(), _type == ExeCommand::READ ? "r" : "w");

    if (_command.empty() || output == nullptr)
        return "";
    while (fgets(buffer.data(), _readSize, output) != nullptr) {
        command_output.append(buffer.data());
    }
    pclose(output);
    return command_output;
}
