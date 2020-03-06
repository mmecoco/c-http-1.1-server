#include "Parser.hpp"
#include "HttpSplit.hpp"

const std::map<std::string, oZ::HTTP::Method> Parser::STR_TO_MAP =
        {{"GET",     oZ::HTTP::Method::Get},
         {"HEAD",    oZ::HTTP::Method::Head},
         {"POST",    oZ::HTTP::Method::Post},
         {"PUT",     oZ::HTTP::Method::Put},
         {"DELETE",  oZ::HTTP::Method::Delete},
         {"TRACE",   oZ::HTTP::Method::Trace},
         {"CONNECT", oZ::HTTP::Method::Connect},
         {"OPTION",  oZ::HTTP::Method::Option}};

Parser::Parser()
{
    std::cout << "ctor => Parser" << std::endl;
}

void Parser::onRegisterCallbacks(oZ::Pipeline &pipeline)
{
    pipeline.registerCallback(
            oZ::State::Parse,
            oZ::Priority::ASAP,
            this, &Parser::onParseHeader
    );
}

bool Parser::onParseHeader(oZ::Context &context)
{
    std::cout << "Parser is in place" << std::endl;
    auto &request = context.getRequest();
    auto &header = request.getHeader();
    auto &body = request.getBody();
    auto &uri = request.getURI();
    auto &query = request.getQueryParameters();
    std::string input;
    std::string default_file = "index.html"; // TODO : hard coded
    auto byteArray = context.getPacket().getByteArray();

    for (auto it : byteArray)
        input += it;
    Utils::HttpSplit splitor(input);
    body = splitor.getBody();
    for (auto it : splitor.getContentMap())
        header.set(it.first, std::move(it.second));
    request.setMethod(STR_TO_MAP.at(splitor.getMethod()));
    splitQueryUri(query, uri);
    uri = "www" + splitor.getPath(); // Jooyoung
    //uri = context.getMetadata<std::string>("root_dir") + splitor.getPath();
    if (uri[uri.length() - 1] == '/')
        uri += default_file;
    return (true);
}

bool Parser::splitQueryUri(oZ::HTTP::QueryParameters &query, oZ::HTTP::URI &uri)
{
    /// www/?name=toto&age=22
    try {
        std::string query_string = uri.substr(uri.find('?') + 1, uri.length());
        uri = uri.substr(0, uri.find('?'));
        std::stringstream ss(query_string);
        std::string current;

        while (std::getline(ss, current, '&')) {
            query.insert(std::make_pair(current.substr(0, current.find('=')),
                                        current.substr(current.find('=') + 1, current.length())));
        }
    } catch (const std::exception &) {
        return false;
    }
    return true;
}
