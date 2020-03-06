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

class Put : public oZ::IModule {
public:
    Put();
    ~Put() override = default;

    [[nodiscard]] const char *getName() const override
    { return "Put"; };
    void onRegisterCallbacks(oZ::Pipeline &pipeline) override;
private:
    bool onPut(oZ::Context &context);
};

OPEN_ZIA_MAKE_ENTRY_POINT(Put);
