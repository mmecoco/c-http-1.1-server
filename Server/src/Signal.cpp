/*
** EPITECH PROJECT, 2020
** CPP_zia_2019
** File description:
** Signal.cpp
*/

#include <iostream>

#include "Signal.hpp"
#include "HttpServer.hpp"

extern "C"
void signal_handler(int signal)
{
    if (signal == SIGINT) {
        if (!HttpServer::Running) {
            // Interrupt press twice, must exit quickly.
            std::cerr << "Graceful exit aborted.\n";
            exit(SIGINT);
        }

        std::cout << "Server stop requested, gracefully stopping..." << std::endl;
        HttpServer::getInstance()->stop_server();
    }
}

namespace Signals {
    void register_signals()
    {
        signal(SIGINT, &signal_handler);
    }
}
