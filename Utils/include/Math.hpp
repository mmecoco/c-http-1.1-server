/*
** EPITECH PROJECT, 2020
** CPP_zia_2019
** File description:
** Math.hpp
*/

#pragma once

#include <cmath>
#include <sstream>
#include <iomanip>
#include <iostream>

namespace Utils {

    template<typename T>
    constexpr inline T log_base(T base, T value)
    {
        return std::log(value) / std::log(base);
    }

    inline std::string human_readable_size(std::uintmax_t size)
    {
        static constexpr std::size_t SIZES = 9;
        static constexpr const char *const SIZE_SUFFIX[SIZES] = {"o", "Ko", "Mo", "Go", "To", "Po", "Eo", "Zo", "Yo"};
        auto size_format = static_cast<long double>(size);
        std::ostringstream oss{};

        auto index = static_cast<decltype(size)>(log_base(1024.0l, size_format));
        for (decltype(index) i = 0; i < index; ++i)
            size_format /= 1024.0l;

        oss << std::setprecision(2) << std::fixed << size_format << ' ' << SIZE_SUFFIX[index];
        return oss.str();
    }
}
