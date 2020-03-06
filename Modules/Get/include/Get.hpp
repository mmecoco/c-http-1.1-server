#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <algorithm>
#include <iterator>
#include <filesystem>

#include <openZia/IModule.hpp>
#include <openZia/Pipeline.hpp>

#include "FileReader.hpp"

class Get : public oZ::IModule {
public:
    Get();
    ~Get() override = default;

    [[nodiscard]] const char *getName() const override
    { return "Get"; };
    void onRegisterCallbacks(oZ::Pipeline &pipeline) override;
private:
    bool onGet(oZ::Context &context);

    static std::string _getContentType(std::string uri) noexcept;
};

OPEN_ZIA_MAKE_ENTRY_POINT(Get);
