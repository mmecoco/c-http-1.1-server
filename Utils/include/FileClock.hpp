/*
** EPITECH PROJECT, 2020
** CPP_zia_2019
** File description:
** File.hpp
*/

#pragma once

#include <chrono>
#include <filesystem>

namespace Utils {
    template<typename TimePoint>
    std::time_t file_clock_to_time_t(TimePoint timePoint)
    {
        auto system_time = std::chrono::time_point_cast<std::chrono::system_clock::duration>(
                timePoint - TimePoint::clock::now()
                + std::chrono::system_clock::now());
        return std::chrono::system_clock::to_time_t(system_time);
    }
}
