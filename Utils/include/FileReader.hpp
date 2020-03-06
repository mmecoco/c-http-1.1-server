/*
** EPITECH PROJECT, 2020
** CPP_zia_2019
** File description:
** FileReader.hpp
*/

#pragma once

#include <string>
#include <fstream>
#include <filesystem>

namespace Utils {
    class FileReader {
    private:
        FileReader() = default;
    public:
        static std::string read(const std::string &path)
        {
            std::cout << "READING " << path << std::endl;
            std::ifstream file_stream(path);

            if (!file_stream.is_open() && !std::filesystem::is_directory(path)) {
                std::cout << "File is not readable" << std::endl;
                throw (std::runtime_error("File is not Opened"));
            }
            std::string str((std::istreambuf_iterator<char>(file_stream)),
                            std::istreambuf_iterator<char>());
            return str;
        }
    };
}
