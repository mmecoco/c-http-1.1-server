#pragma once

#include <iostream>
#include <vector>
#include <sstream>
#include <algorithm>
#include <iterator>

#include <openZia/IModule.hpp>
#include <openZia/Pipeline.hpp>

#include "FileReader.hpp"

class Option : public oZ::IModule {
public:
    Option();
    ~Option() override = default;

    [[nodiscard]] const char *getName() const override
    { return "option"; };
    void onRegisterCallbacks(oZ::Pipeline &pipeline) override;
private:
    bool onOption(oZ::Context &context);
};

OPEN_ZIA_MAKE_ENTRY_POINT(Option);
