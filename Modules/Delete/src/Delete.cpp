#include <filesystem>

#include "Delete.hpp"

Delete::Delete()
{
    std::cout << "ctor => Delete" << std::endl;
}

void Delete::onRegisterCallbacks(oZ::Pipeline &pipeline)
{
    pipeline.registerCallback(
            oZ::State::AfterParse,
            oZ::Priority::High,
            this, &Delete::onDelete
    );
}

bool Delete::onDelete(oZ::Context &context)
{
    std::cout << "Delete is in place" << std::endl;
    const auto &request = context.getRequest();
    const auto &uri = request.getURI();
    auto &response = context.getResponse();

    if (request.getMethod() != oZ::HTTP::Method::Delete)
        return true;

    const auto &path = uri;

    try {
        std::cerr << "DELETE => " << path << std::endl;
        if (std::filesystem::exists(path) && std::filesystem::is_regular_file(path)) {
            std::filesystem::remove(path);
            response.setCode(oZ::HTTP::Code::OK);
            return true;
        }
    } catch (const std::filesystem::filesystem_error &e) {
        std::cerr << e.what() << std::endl;
    }
    response.setCode(oZ::HTTP::Code::NoContent);
    return true;
}

const char *Delete::getName() const
{
    return "Delete";
}
