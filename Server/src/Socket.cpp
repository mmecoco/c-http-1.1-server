#include <iostream>
#include <utility>

#include "Socket.hpp"

#if defined(SYSTEM_WINDOWS)
#define close _close
#endif

Socket::Socket(std::string ip, unsigned short port) : m_ip(std::move(std::move(ip))), m_port(port)
{
    int opt = 1;

    m_addrlen = sizeof(m_address);

#ifdef __linux__
    if ((m_server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket");
    }
    if (setsockopt(m_server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
        perror("setsockopt");
#elif _WIN32
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        perror("startup");
    }
    if((m_socket = socket(AF_INET , SOCK_STREAM , 0 )) == INVALID_SOCKET) {
        perror("socket");
    }
#endif
    m_address.sin_family = AF_INET;
    m_address.sin_addr.s_addr = INADDR_ANY;
    m_address.sin_port = htons(m_port);
#ifdef __linux__

    if (!bindSocket())
        perror("bind");
    if (!listenSocket())
        perror("listen");
#endif
}

bool Socket::bindSocket()
{
    return bind(m_server_fd, (struct sockaddr *) &m_address, sizeof(m_address)) >= 0;
}

bool Socket::listenSocket()
{
    return listen(m_server_fd, 5) >= 0;
}

bool Socket::acceptSocket()
{
#ifdef __linux__
    std::cout << "Accepting\n";
    if ((m_socket = accept(m_server_fd, (struct sockaddr *) &m_address, (socklen_t *) &m_addrlen)) < 0) {
        return (false);
    } else {
        std::cout << "Accepted in fd == " << m_socket << std::endl;
        return (true);
    }
#elif _WIN32
    return (connectSocket());
#endif
}

bool Socket::connectSocket()
{
    return connect(m_socket, (struct sockaddr *) &m_address, sizeof(m_address)) >= 0;
}

std::string Socket::readSocket()
{
    int bytesRead;

#ifdef __linux__
    bytesRead = read(m_socket, m_data, 1024);
    if (bytesRead == -1) {
        std::cerr << "Reading from socket has an error " << std::endl;
        return "";
    }
#elif _WIN32
    bytesRead = recv(m_socket, m_data, 1024, 0);
#endif
    std::string result(m_data, bytesRead);
    std::memset(m_data, 0, 1024);
    return result;
}

std::int64_t Socket::sendSocket(const std::string &target)
{
    return (send(m_socket, target.c_str(), static_cast<int>(target.length()), 0));
}

bool Socket::closeSocket()
{
    int close_accept = close(m_server_fd);

    if (!m_keep_alive)
        return false;
    if (close_accept == -1) {
        std::cout << "Failed to close accept socket" << std::endl;
    }
    return (close_accept == 0);
}

oZ::FileDescriptor Socket::getFd() const noexcept
{
    return (m_socket);
}

void Socket::setLife(bool isKeepAlive)
{
    m_keep_alive = isKeepAlive;
}

bool Socket::getLife() const
{
    return m_keep_alive;
}
