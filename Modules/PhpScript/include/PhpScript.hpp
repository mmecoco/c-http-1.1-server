#pragma once

#include <iostream>
#include <vector>
#include <sstream>
#include <algorithm>

#include <openZia/IModule.hpp>
#include <openZia/Pipeline.hpp>

class PhpScript : public oZ::IModule {
public:
    PhpScript();
    ~PhpScript() override = default;

    [[nodiscard]] const char *getName() const override
    { return "PhpScript"; };
    void onRegisterCallbacks(oZ::Pipeline &pipeline) override;
private:
    bool onParseScript(oZ::Context &context);
    bool interpretGet(oZ::Context &context);
    bool interpretPost(oZ::Context &context);
    bool setFileFromURI(const std::string &URI);
    std::string queryParamsToStr(oZ::Context &context);
    std::string getBodyQuery(oZ::Context &context);
    void fillHeaderFromString(oZ::HTTP::Header &responseHeader, const std::string &header, size_t bodyLength);

    std::string _fileToParse;
};

OPEN_ZIA_MAKE_ENTRY_POINT(PhpScript);