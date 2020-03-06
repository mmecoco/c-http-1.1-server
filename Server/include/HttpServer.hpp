#pragma once

#include <vector>
#include <memory>
#include <iostream>
#include <thread>
#include <future>
#include <unordered_map>
#include <cstdio>
#include <ctime>

#include <openZia/Pipeline.hpp>
#include <openZia/IModule.hpp>
#include <openZia/Endpoint.hpp>
#include <openZia/Context.hpp>

#include "Socket.hpp"
#include "FileReader.hpp"
#include "HttpSplit.hpp"

class HttpServer {
public:
    HttpServer(unsigned short port, const std::string &ip);
    ~HttpServer();

    static const std::map<oZ::HTTP::Code, std::string> CODE_TO_NAME;
    static bool Running;

    void start_server();
    void stop_server();

    static HttpServer *getInstance();
private:
    void start_accept();
    bool accept(const std::shared_ptr<Socket> &socket);
    std::string create_response(const oZ::Context &context, const std::shared_ptr<Socket> &socket);
    static std::string get_current_date();

    //============================================================
    // handlers functions that allows us to make callbacks
    // for the accept, read and write
    //============================================================

    void readHttp(const std::shared_ptr<Socket> &socket, oZ::Context &context);

    //============================================================
    // the functions that is required in the Openzia github
    //============================================================

    void onClientConnected(oZ::Context &context, const std::shared_ptr<Socket>& socket);
    void onClientDisconnected(oZ::Context &context, const std::shared_ptr<Socket> &socket);
    void onPacketReceived(oZ::Context &context, const std::shared_ptr<Socket>& socket);
    void sendResponseToClient(const oZ::Context &context, const std::shared_ptr<Socket> &socket);
    std::string closeOrNot(bool life);

    static HttpServer *Instance;

    std::vector<std::shared_ptr<Socket>> m_sockets{};
    std::vector<std::thread> m_threads{};

    int session_id;
    oZ::Pipeline m_pipeline;
    char m_data[1024]{};
    std::string m_ip;
    oZ::Port m_port;
    oZ::ByteArray m_array{};
};
