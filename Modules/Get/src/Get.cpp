#include "Get.hpp"
#include "FileReader.hpp"

Get::Get()
{
    std::cout << "ctor => Get" << std::endl;
}

void Get::onRegisterCallbacks(oZ::Pipeline &pipeline)
{
    pipeline.registerCallback(
            oZ::State::AfterParse,
            oZ::Priority::High,
            this, &Get::onGet
    );
}

std::string Get::_getContentType(std::string ext) noexcept
{
    static constexpr const char *arr[] =
            {"application/javascript", "application/ogg", "application/pdf", "application/json", "application/xml",
             "application/zip", "audio/mpeg", "image/gif", "image/jpeg", "image/png", "image/tiff", "image/x-icon",
             "image/svg+xml", "text/css", "text/csv", "text/html", "text/javascript", "text/plain", "text/xml",
             "video/mpeg", "video/mp4", "video/webm"};

    if (!ext.empty() && ext[0] == '.')
        ext = ext.substr(1);

    // "Dumb" conversion
    size_t index = 0;
    for (std::string type : arr) {
        type = type.substr(type.find('/') + 1);
        if (type == ext)
            return arr[index];
        ++index;
    }

    // Specifications
    if (ext == "js") return "application/javascript";
    if (ext == "ico") return "image/x-icon";
    if (ext == "svg") return "image/svg+xml";

    // Default
    return "text/plain";
}

bool Get::onGet(oZ::Context &context)
{
    std::cout << "Get is in place" << std::endl;
    auto &request = context.getRequest();
    auto &response = context.getResponse();
    auto &uri = request.getURI();
    if (request.getMethod() == oZ::HTTP::Method::Get) {
        std::cout << uri << std::endl;
        std::string default_file = "index.html"; // Jooyoung
        //const auto &default_file = context.getMetadata<std::string>("default_file");
        if (uri[uri.length() - 1] == '/')
            uri += default_file;
        std::string real_path = uri;
        const auto ext = std::filesystem::path(real_path).extension().string();

        if (uri.find(".php") != std::string::npos)
            return true;
        // TODO

        auto &body = response.getBody();
        auto &header = response.getHeader().getStringMultimap();
        try {
            body = Utils::FileReader::read(real_path);
            response.setCode(oZ::HTTP::Code::OK);
            header.insert(std::make_pair("Content-Type", _getContentType(ext)));
            header.insert(std::make_pair("Content-Length", std::to_string(body.length())));
        } catch (const std::exception &) {
            if (std::filesystem::exists(real_path) && !std::filesystem::is_directory(real_path)) {
                response.setCode(oZ::HTTP::Code::Forbidden);
                response.getReason() = "You are not allowed to access this file";
            } else if (std::filesystem::exists(real_path) && std::filesystem::is_directory(real_path)) {
                response.setCode(oZ::HTTP::Code::OK);
            } else {
                response.setCode(oZ::HTTP::Code::NotFound);
                response.getReason() = "Could not find \"" + uri.substr(uri.find('/')) + "\" on this server";
            }
        }
        return true;
    }
    return true;
}
