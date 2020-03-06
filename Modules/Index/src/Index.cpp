#include <iostream>
#include <filesystem>

#include "Math.hpp"
#include "Index.hpp"
#include "FileClock.hpp"
#include "Stylesheet.hpp"

Index::Index()
{
    std::cout << "ctor => Index" << std::endl;
}

void Index::onRegisterCallbacks(oZ::Pipeline &pipeline)
{
    pipeline.registerCallback(
            oZ::State::AfterParse,
            oZ::Priority::Low,
            this, &Index::_callback
    );
}

void Index::_buildLink(std::ostream &os, const std::filesystem::directory_entry &entry)
{
    auto url = entry.path().string();

    url = url.substr(url.find('/'));
    os << "<a href=" << std::quoted(url) << ">" << entry.path().filename().string() << "</a>";
}

void Index::_buildEntry(std::ostream &os, const std::filesystem::directory_entry &entry)
{
    const auto size = entry.is_directory() ? 0 : entry.file_size();
    const auto time = Utils::file_clock_to_time_t(entry.last_write_time());

    os << "<tr>\n";
    os << "<td>";
    _buildLink(os, entry);
    os << "</td>\n";

    if (entry.is_directory())
        os << "<td> </td>\n";
    else
        os << "<td>" << Utils::human_readable_size(size) << "</td>\n";

    os << "<td>" << std::ctime(&time) << "</td>\n";
    os << "</tr>\n";
}

void Index::_buildIndex(oZ::HTTP::Response &response)
{
    std::ostringstream oss{};
    auto &body = response.getBody();
    const auto relative = _fullPath.substr(_fullPath.find('/'));

    oss << "<!DOCTYPE html>\n"
           "<html>\n"
           "<head>\n"
           "<title>Index of " << relative << "</title>\n"
        << Stylesheet::default_style()
        << "<body>\n";
    oss << "<h1>Content of " << relative << "</h1>\n";

    oss << "<table>\n";
    oss << "<tr>\n"
           "<th>NAME</th>\n"
           "<th>SIZE</th>\n"
           "<th>LAST MODIFIED</th>\n"
           "</tr>\n";
    for (const auto &entry : std::filesystem::directory_iterator(_fullPath))
        _buildEntry(oss, entry);
    oss << "</table>\n";

    oss << "</body>\n"
           "</head>\n"
           "</html>";

    oss << std::flush;
    body = oss.str();
    body.shrink_to_fit();

    response.getHeader().set("Content-Length", std::to_string(body.length()));
}

bool Index::_callback(oZ::Context &context)
{
    std::cout << "Index is in place" << std::endl;
    const auto &request = context.getRequest();
    const auto &uri = request.getURI();
    auto &response = context.getResponse();

    if (request.getMethod() != oZ::HTTP::Method::Get)
        return true;

    const auto &path = uri;

    try {
        if (std::filesystem::exists(path) && std::filesystem::is_directory(path)) {
            if (true) {
            //if (context.getMetadata<bool>("enable_index")) { // jooyoung
                _fullPath = path;
                _buildIndex(response);
                response.setCode(oZ::HTTP::Code::OK);
                return true;
            } else {
                response.setCode(oZ::HTTP::Code::Forbidden);
                response.getReason() = "Access to index denied";
                return true;
            }
        }
    } catch (const std::filesystem::filesystem_error &e) {
        std::cerr << e.what() << std::endl;
        response.setCode(oZ::HTTP::Code::InternalServerError);
        response.getReason() = "Something went wrong";
    }
    std::cout << "End of Index" << std::endl;
    return true;
}

const char *Index::getName() const
{
    return "Index";
}
