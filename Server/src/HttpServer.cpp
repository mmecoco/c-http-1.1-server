#include "HttpServer.hpp"
#include "code_to_string.hpp"
#include "HttpSplit.hpp"
#include <fstream>
#include <utility>
#include "HttpCode.hpp"

bool HttpServer::Running = true;
HttpServer *HttpServer::Instance = nullptr;

HttpServer::HttpServer(unsigned short port, const std::string &ip)
        : session_id(0), m_pipeline("ModuleDir", "ConfigDir"), m_ip(ip), m_port(port)
{
    Instance = this;
    std::cout << "Starting server => http://" << ip << ':' << port << std::endl;
    std::cout << "                => https://" << ip << ':' << port << std::endl;
    m_pipeline.loadModules();
}

HttpServer::~HttpServer()
{
    Instance = nullptr;
    m_pipeline.clear();
    m_threads.clear();
}

void HttpServer::start_server()
{
    while (Running)
        start_accept();
}

void HttpServer::stop_server()
{
    Running = false;

    std::cout << "Shutting down sockets" << std::endl;
    for (auto &socket : m_sockets)
        socket->closeSocket();

    std::cout << "Destroying threads" << std::endl;
    m_threads.clear();
}

bool HttpServer::accept(const std::shared_ptr<Socket> &socket)
{
    oZ::Endpoint endpoint(m_ip, m_port);
    oZ::Context context(oZ::Packet(oZ::ByteArray(), endpoint, socket->getFd()));
    onClientConnected(context, socket);

    if (socket->getLife()) {
        readHttp(socket, context);
    } else
        return socket->closeSocket();
    return true;
}

void HttpServer::start_accept()
{
    auto socket = std::make_shared<Socket>(m_ip, m_port);
    m_sockets.push_back(std::move(socket));
    m_sockets.back()->acceptSocket();

    if (Running) {
        std::thread thd(&HttpServer::accept, this, m_sockets.back());
        thd.detach();
        m_threads.push_back(std::move(thd));
        std::cout << "detached" << std::endl;
    }
}

void HttpServer::readHttp(const std::shared_ptr<Socket> &socket, oZ::Context &context)
{
    bool encryption = false;
    bool keep_alive = false;

    m_array.clear();
    auto got = socket->readSocket();

    if (got.empty())
        return;
    std::cout << "|||||||||||||||||||||||||||||||||||||||||||" << std::endl;
    std::cout << got << std::endl;
    std::cout << "|||||||||||||||||||||||||||||||||||||||||||" << std::endl;
    Utils::HttpSplit splitor(got);
    auto ContentMap = splitor.getContentMap();
    std::cout << "$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$" << std::endl;
    for (auto it = ContentMap.lower_bound("Connection"); it != ContentMap.upper_bound("Connection"); it++) {
        if (it->second == "keep-alive") {
            keep_alive = true;
            std::cout << "PLZ ALIVE ME" << std::endl;
        } else {
            std::cout << "Got " << it->second << " instead of keep alive!" << std::endl;
        }
    }
    std::cout << "$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$" << std::endl;
    std::cout << "Alive" << std::endl;
    if (splitor.getPath().find("https") != std::string::npos)
        encryption = true;
    auto &packet = context.getPacket();
    packet.setEncryption(encryption);
    socket->setLife(keep_alive);
    auto &byte_array = context.getPacket().getByteArray();
    for (auto it : got) {
        byte_array.push_back(it);
    }
    oZ::Endpoint endpoint(m_ip, m_port);
    onPacketReceived(context, socket);
}

void HttpServer::onClientConnected(oZ::Context &context, const std::shared_ptr<Socket> &socket)
{
    m_pipeline.onConnection(context);
    readHttp(socket, context);
}

void HttpServer::onClientDisconnected(oZ::Context &context, const std::shared_ptr<Socket> &socket)
{
    (void) socket;
    m_pipeline.onDisconnection(context);
}

void HttpServer::onPacketReceived(oZ::Context &context, const std::shared_ptr<Socket> &socket)
{
    m_pipeline.runPipeline(context);
    std::cout << "[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[" << std::endl;
    sendResponseToClient(context, socket);
}

void HttpServer::sendResponseToClient(const oZ::Context &context, const std::shared_ptr<Socket> &socket)
{
    std::cout << "writing" << std::endl;
    auto &method = context.getRequest();
    std::cout << method.getURI() << std::endl;
    socket->sendSocket(create_response(context, socket));
}

std::string HttpServer::get_current_date()
{
    char buf[1000];
    time_t now = time(nullptr);
    struct tm tm = *gmtime(&now);
    std::string timer;

    strftime(buf, sizeof buf, "%a, %d %b %Y %H:%M:%S %Z", &tm);
    timer += buf;
    return timer;
}

std::string HttpServer::create_response(const oZ::Context &context, const std::shared_ptr<Socket> &socket)
{
    using namespace std::literals::string_literals;

    auto response = context.getResponse();
    auto body = response.getBody();
    auto code = response.getCode();
    auto header = response.getHeader().getStringMultimap();
    std::string result = "HTTP/1.1 ";
    result += std::to_string(static_cast<int>(code)) + " ";
    result += Utils::codeToString(code) + "\n"s;
    std::unordered_map<std::string, std::string> http_headers{{"Server",     "Zia"},
                                                              {"Connection", closeOrNot(socket->getLife())}};

    for (const auto &it : http_headers) {
        result += it.first;
        result += ":";
        result += it.second;
        result += "\n";
    }
    for (const auto &it : header) {
        result += it.first;
        result += ":";
        result += it.second;
        result += "\n";
    }
    result += "\n";
    result += body;
    std::cout << "<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<\n";
    std::cout << result << std::endl;
    std::cout << "<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<\n";
    return result;
}

HttpServer *HttpServer::getInstance()
{
    return Instance;
}

std::string HttpServer::closeOrNot(bool life)
{
    if (life) {
        return ("Closed");
    } else
        return ("Closed");
}
