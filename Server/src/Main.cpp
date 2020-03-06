/*
** EPITECH PROJECT, 2020
** CPP_zia_2019
** File description:
** Main.cpp
*/

#include <optional>
#include <iostream>
#include <string_view>

#include "Signal.hpp"
#include "ServerConfig.hpp"
#include "HttpServer.hpp"

static void print_help(const char *const prg_name) noexcept
{
    std::cout << "USAGE\n"
                 "    " << prg_name << " [-h|--help] [port [host]]\n"
              << "\n"
                 "OPTIONS\n"
                 "    --help    shows this message\n"
                 "    port      overrides the config port\n"
                 "    host      overrides the config host, needs port as well\n";
}

static std::optional<ServerConfig::ConfigData> get_config(int ac, const char *const *const av) noexcept
{
    auto config = ServerConfig().getConfigData();

    if (ac > 1) {
        if (std::string_view{av[1]} == "--help" || std::string_view{av[1]} == "-h") {
            print_help(av[0]);
            return std::nullopt;
        }
        config.port = std::atoi(av[1]);
        if (ac == 3) {
            config.host = av[2];
        } else if (ac > 3) {
            std::cerr << "Too many arguments\n";
            return std::nullopt;
        }
    }
    return std::make_optional(config);
}

int main(const int ac, const char *const *const av)
{
    auto config = get_config(ac, av);

    if (!config)
        return 42;

    try {
        Signals::register_signals();
        HttpServer s(config->port, config->host);

        s.start_server();
    } catch (const std::exception &e) {
        std::cerr << "error: " << e.what() << '\n';
        return 84;
    }
    return 0;
}
