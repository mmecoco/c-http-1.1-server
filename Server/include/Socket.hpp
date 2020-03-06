#pragma once

#ifdef __linux__

#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

#elif _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <winsock.h>
#include <io.h>
#pragma comment(lib, "ws2_32.lib")
#endif

#include <string>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cstdint>

#include <openZia/Endpoint.hpp>

class Socket {
public:
    Socket(std::string ip, unsigned short port);
    ~Socket() = default;

    std::string readSocket();
    std::int64_t sendSocket(const std::string &target);
    bool acceptSocket();
    [[nodiscard]] oZ::FileDescriptor getFd() const noexcept;
    bool closeSocket();
    void setLife(bool isKeepAlive);
    [[nodiscard]] bool getLife() const;

private:
    bool bindSocket();
    bool listenSocket();
    bool connectSocket();

    std::string m_ip;
    int m_port;
    char m_data[1024]{};
    int m_server_fd{};
    oZ::FileDescriptor m_socket{};
    struct sockaddr_in m_address{};
    int m_addrlen{};
    bool m_keep_alive{};
};
