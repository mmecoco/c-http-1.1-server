#include "Put.hpp"

Put::Put()
{
    std::cout << "ctor => Put" << std::endl;
}

void Put::onRegisterCallbacks(oZ::Pipeline &pipeline)
{
    pipeline.registerCallback(
            oZ::State::AfterParse,
            oZ::Priority::High,
            this, &Put::onPut
    );
}

bool Put::onPut(oZ::Context &context)
{
    // file exist and ok => OK
    // file no exist and ok => Created
    // file exist but no body  => No Content
    std::cout << "Put is in place" << std::endl;
    auto &request = context.getRequest();
    auto &response = context.getResponse();
    auto uri = request.getURI();
    auto req_body = request.getBody();

    if (request.getMethod() == oZ::HTTP::Method::Put) {
        bool ifFileExist = std::filesystem::exists(uri);
        std::ofstream fs(uri);

        if (ifFileExist) {
            if (req_body.empty())
                response.setCode(oZ::HTTP::Code::NoContent);
            else
                response.setCode(oZ::HTTP::Code::OK);
            fs << req_body;
        } else {
            response.setCode(oZ::HTTP::Code::Created);
            fs << req_body;
        }
    }
    return true;
}
