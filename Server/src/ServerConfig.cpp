/*
** EPITECH PROJECT, 2020
** CPP_zia_2019
** File description:
** Config.cpp
*/

#include <fstream>
#include <iostream>

#include "ServerConfig.hpp"

const ServerConfig::ConfigData ServerConfig::DEFAULT_CONFIG{"127.0.0.1", 4242};

ServerConfig::ServerConfig(const std::string &path)
{
    _loadFromFile(path);
}

void ServerConfig::_loadFromFile(const std::string &path)
{
    std::ifstream file{path};

    if (!file) {
        std::cerr << "Could not open config file " << path << '\n';
        std::cerr << "Using the default config instead\n";
        _data = DEFAULT_CONFIG;

        _rawData["host"] = _data.host;
        _rawData["port"] = _data.port;
        _writeConfig();
    } else {
        file >> _rawData;
        _data.host = _rawData["host"];
        _data.port = _rawData["port"];
    }
}

void ServerConfig::_writeConfig(const std::string &path) const noexcept
{
    std::ofstream file{path};

    if (!file) {
        std::cerr << "Could not create file " << path << ", ignoring...\n";
        return;
    }
    file << _rawData;
}

ServerConfig::ConfigData ServerConfig::getConfigData() const noexcept
{
    return _data;
}
