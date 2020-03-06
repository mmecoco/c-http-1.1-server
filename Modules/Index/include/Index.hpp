#pragma once

#include <openZia/IModule.hpp>
#include <openZia/Pipeline.hpp>

class Index : public oZ::IModule {
public:
    Index();
    ~Index() override = default;

    [[nodiscard]] const char *getName() const override;
    void onRegisterCallbacks(oZ::Pipeline &pipeline) override;
private:
    bool _callback(oZ::Context &context);
    void _buildLink(std::ostream &os, const std::filesystem::directory_entry &entry);
    void _buildEntry(std::ostream &os, const std::filesystem::directory_entry &entry);
    void _buildIndex(oZ::HTTP::Response &response);

    std::string _fullPath{};
};

OPEN_ZIA_MAKE_ENTRY_POINT(Index);
