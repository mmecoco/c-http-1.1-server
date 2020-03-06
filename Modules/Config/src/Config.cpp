#include <iostream>
#include <fstream>

#include "Config.hpp"

const Config::ConfigData Config::DEFAULT_CONFIG{"www", "index.html", true};

Config::Config()
{
    std::cout << "ctor => Config" << std::endl;
}

const char *Config::getName() const
{
    return "Config";
}

void Config::onRegisterCallbacks(oZ::Pipeline &pipeline)
{
    pipeline.registerCallback(
            oZ::State::BeforeParse,
            oZ::Priority::ASAP,
            this, &Config::_callback
    );
}

void Config::onLoadConfigurationFile(const std::string &dir)
{
    const auto path = dir + CONFIG_FILE;
    std::ifstream file{path};

    if (!file) {
        std::cerr << "Could not open config file " << path << '\n';
        std::cerr << "Using the default config instead\n";
        _data = DEFAULT_CONFIG;

        _rawData["root_dir"] = _data.root_dir;
        _rawData["default_file"] = _data.default_file;
        _rawData["enable_index"] = _data.enable_index;
        _writeConfig(dir);
    } else {
        file >> _rawData;
        _data.root_dir = _rawData["root_dir"];
        _data.default_file = _rawData["default_file"];
        _data.enable_index = _rawData["enable_index"];
    }
}

bool Config::_callback(oZ::Context &context)
{
    context.setMetadata("root_dir", _data.root_dir);
    context.setMetadata("default_file", _data.default_file);
    context.setMetadata("enable_index", _data.enable_index);
    return true;
}

void Config::_writeConfig(const std::string &dir) const noexcept
{
    const auto path = dir + CONFIG_FILE;
    std::ofstream file{path};

    if (!file) {
        std::cerr << "Could not create file " << path << ", ignoring...\n";
        return;
    }
    file << _rawData;
}
