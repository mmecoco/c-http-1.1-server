#include "Option.hpp"

Option::Option()
{
    std::cout << "ctor => Option" << std::endl;
}

void Option::onRegisterCallbacks(oZ::Pipeline &pipeline)
{
    pipeline.registerCallback(
            oZ::State::AfterParse,
            oZ::Priority::High,
            this, &Option::onOption
    );
}

bool Option::onOption(oZ::Context &context)
{
    std::cout << "Option is in place" << std::endl;
    auto &request = context.getRequest();
    auto &response = context.getResponse();
    auto &uri = request.getURI();

    if (request.getMethod() == oZ::HTTP::Method::Option) {

        auto &header = response.getHeader().getStringMultimap();
        try {
            auto file = Utils::FileReader::read(uri);
            response.setCode(oZ::HTTP::Code::OK);
            header.insert(std::make_pair("Allow", "OPTIONS, GET, HEAD, POST, DELETE, TRACE")); // Todo : Hard coded
        } catch (const std::exception &) {
            response.setCode(oZ::HTTP::Code::NoContent);
            return false;
        }
        return true;
    }
    return true;
}
