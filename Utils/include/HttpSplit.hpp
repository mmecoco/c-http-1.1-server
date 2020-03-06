#pragma once

#include <string>
#include <iostream>
#include <map>
#include <vector>
#include <sstream>

namespace Utils {
    class HttpSplit {
    private:
        std::string m_original;
        std::string m_path{};
        std::string m_method{};
        std::string m_version{};
        std::vector<std::string> m_all{};
        std::vector<std::string> m_first{};
        std::multimap<std::string, std::string> m_map{};
        std::string m_body{};

        void setPath()
        {
            m_path = m_first.at(1);
        }

        void setMethod()
        {
            m_method = m_first.at(0);
        }

        void setVersion()
        {
            m_version = m_first.at(2);
        }

        void setContent()
        {
            bool pass_body = false;

            for (const auto &it : m_all) {
                auto pos = it.find(':');
                if (pos == std::string::npos && !pass_body)
                    pass_body = true;

                if (pass_body) {
                    m_body += it + "\n";
                } else {
                    std::string key = it.substr(0, pos);
                    std::string value = it.substr(pos + 2, (it.length()));
                    value = value.substr(0, value.length() - 1);
                    m_map.insert(std::make_pair(key, value));
                    std::cout << "inserting " << key << " with " << value << value.length() << std::endl;
                }
            }
        }

    public:
        explicit HttpSplit(std::string request)
                : m_original(std::move(request))
        {
            std::stringstream ss(m_original);
            std::string current;
            std::string line;

            while (std::getline(ss, current, '\n')) {
                if (current.length() != 1 || isprint(current.c_str()[0]))
                    m_all.push_back(current);
            }

            std::stringstream ssl(m_all.at(0));

            while (std::getline(ssl, line, ' ')) {
                if (current.length() != 1 || isprint(current.c_str()[0]))
                    m_first.push_back(line);
            }
            m_all.erase(m_all.begin());

            try {
                setPath();
                setMethod();
                setVersion();
                setContent();
            } catch (const std::exception &) {
                std::cerr << "not a correct file" << std::endl;
            }

            std::cout << m_body << std::endl;
        }

        ~HttpSplit() = default;

        [[nodiscard]] std::string getPath() const noexcept
        { return m_path; };

        [[nodiscard]] std::string getMethod() const noexcept
        { return m_method; };

        [[nodiscard]] std::string getHttpVersion() const noexcept
        { return m_version; };

        [[nodiscard]] std::multimap<std::string, std::string> getContentMap() const noexcept
        { return m_map; };

        [[nodiscard]] std::string getBody() const noexcept
        { return m_body; };
    };
}
