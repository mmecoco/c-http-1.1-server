#pragma once

#include <openZia/IModule.hpp>
#include <openZia/Pipeline.hpp>

class Error : public oZ::IModule {
public:
    Error();
    ~Error() override = default;

    [[nodiscard]] const char *getName() const override;
    void onRegisterCallbacks(oZ::Pipeline &pipeline) override;
private:
    bool _callback(oZ::Context &context);
};

OPEN_ZIA_MAKE_ENTRY_POINT(Error);
