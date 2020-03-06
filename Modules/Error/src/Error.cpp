#include <sstream>
#include <iostream>

#include "Error.hpp"
#include "HttpCode.hpp"
#include "Stylesheet.hpp"

Error::Error()
{
    std::cout << "ctor => Error" << std::endl;
}

void Error::onRegisterCallbacks(oZ::Pipeline &pipeline)
{
    pipeline.registerCallback(
            oZ::State::Completed,
            oZ::Priority::Medium,
            this, &Error::_callback
    );
}

bool Error::_callback(oZ::Context &context)
{
    auto &response = context.getResponse();
    auto &body = response.getBody();
    const auto &code = static_cast<int>(response.getCode());

    if (code >= 400) {
        std::ostringstream oss{};
        const std::string error = std::to_string(code) + " - " + Utils::codeToString(code);

        oss << "<!DOCTYPE html>\n"
               "<html>\n"
               "<head>\n"
               "<title>" << error << "</title>\n"
            << Stylesheet::default_style()
            << "<body>\n"
               "<h1>" << error << "</h1>\n";
        oss << response.getReason() << '\n';
        oss << "</body>\n"
               "</head>\n"
               "</html>";

        oss << std::flush;
        body = oss.str();
        body.shrink_to_fit();

        response.getHeader().set("Content-Length", std::to_string(body.length()));
    }
    return true;
}

const char *Error::getName() const
{
    return "Error";
}
