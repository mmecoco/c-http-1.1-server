#pragma once

#include <iostream>
#include <vector>
#include <sstream>
#include <algorithm>
#include <iterator>

#include <openZia/IModule.hpp>
#include <openZia/Pipeline.hpp>

class Delete : public oZ::IModule {
public:
    Delete();
    ~Delete() override = default;
    [[nodiscard]] const char *getName() const override;
    void onRegisterCallbacks(oZ::Pipeline &pipeline) override;
private:
    bool onDelete(oZ::Context &context);
};

OPEN_ZIA_MAKE_ENTRY_POINT(Delete);
