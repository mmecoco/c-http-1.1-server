/*
** EPITECH PROJECT, 2020
** CPP_zia_2019
** File description:
** Config.hpp
*/

#pragma once

#include <nlohmann/json.hpp>

class ServerConfig {
public:
    struct ConfigData {
        std::string host;
        std::uint16_t port;
    };

    using Json = nlohmann::json;
    static constexpr auto CONFIG_FILE = "config.json";
    static const ConfigData DEFAULT_CONFIG;

    explicit ServerConfig(const std::string &path = CONFIG_FILE);
    ~ServerConfig() = default;

    [[nodiscard]] ConfigData getConfigData() const noexcept;
private:
    Json _rawData{};
    ConfigData _data{};

    void _loadFromFile(const std::string &path = CONFIG_FILE);
    void _writeConfig(const std::string &path = CONFIG_FILE) const noexcept;
};
