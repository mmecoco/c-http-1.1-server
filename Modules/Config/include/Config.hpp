#pragma once

#include <nlohmann/json.hpp>

#include <openZia/IModule.hpp>
#include <openZia/Pipeline.hpp>

class Config : public oZ::IModule {
public:
    struct ConfigData {
        std::string root_dir;
        std::string default_file;
        bool enable_index;
    };

    using Json = nlohmann::json;
    static constexpr auto CONFIG_FILE = "config.json";
    static const ConfigData DEFAULT_CONFIG;

    Config();
    ~Config() override = default;

    [[nodiscard]] const char *getName() const override;
    void onRegisterCallbacks(oZ::Pipeline &pipeline) override;
    void onLoadConfigurationFile(const std::string &dir) override;
private:
    Json _rawData{};
    ConfigData _data{};

    bool _callback(oZ::Context &context);
    void _writeConfig(const std::string &dir) const noexcept;
};

OPEN_ZIA_MAKE_ENTRY_POINT(Config);
