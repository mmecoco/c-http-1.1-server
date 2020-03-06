#pragma once

#include <iostream>
#include <vector>
#include <sstream>
#include <algorithm>
#include <iterator>

#include <openZia/IModule.hpp>
#include <openZia/Pipeline.hpp>

#include "HttpSplit.hpp"

class Parser : public oZ::IModule {
public:
    Parser();
    ~Parser() override = default;

    [[nodiscard]] const char *getName() const override
    { return "Parser"; };
    void onRegisterCallbacks(oZ::Pipeline &pipeline) override;
    static const std::map<std::string, oZ::HTTP::Method> STR_TO_MAP;
private:
    bool onParseHeader(oZ::Context &context);
    bool splitQueryUri(oZ::HTTP::QueryParameters &query, oZ::HTTP::URI &uri);
};

OPEN_ZIA_MAKE_ENTRY_POINT(Parser);
