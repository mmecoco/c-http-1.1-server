#pragma once

#include <iostream>
#include <vector>
#include <sstream>
#include <algorithm>
#include <iterator>

#include <openZia/IModule.hpp>
#include <openZia/Pipeline.hpp>

#include "FileReader.hpp"

class Head : public oZ::IModule {
public:
    Head();
    ~Head() override = default;

    [[nodiscard]] const char *getName() const override
    { return "Head"; };
    void onRegisterCallbacks(oZ::Pipeline &pipeline) override;
private:
    bool onHeader(oZ::Context &context);
};

OPEN_ZIA_MAKE_ENTRY_POINT(Head);
