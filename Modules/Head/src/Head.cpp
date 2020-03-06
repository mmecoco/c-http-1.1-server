#include "Head.hpp"

Head::Head()
{
    std::cout << "ctor => Head" << std::endl;
}

void Head::onRegisterCallbacks(oZ::Pipeline &pipeline)
{
    pipeline.registerCallback(
            oZ::State::AfterParse,
            oZ::Priority::High,
            this, &Head::onHeader
    );
}

bool Head::onHeader(oZ::Context &context)
{
    std::cout << "Head is in place" << std::endl;
    auto &request = context.getRequest();
    auto &response = context.getResponse();
    auto &uri = request.getURI();

    if (request.getMethod() == oZ::HTTP::Method::Head) {
        std::cout << uri << std::endl;
        std::string default_file = "index.html"; // Jooyoung
        //const auto &default_file = context.getMetadata<std::string>("default_file");
        if (uri[uri.length() - 1] == '/')
            uri += default_file;
        std::string real_path = uri;

        auto &header = response.getHeader().getStringMultimap();
        try {
            response.setCode(oZ::HTTP::Code::OK);
            header.insert(std::make_pair("Content-Type", "text/html"));
            header.insert(
                    std::make_pair("Content-Length", std::to_string(Utils::FileReader::read(real_path).length())));
        } catch (const std::exception &) {
            response.setCode(oZ::HTTP::Code::NotFound);
            return false;
        }
        return true;
    }
    return true;
}
