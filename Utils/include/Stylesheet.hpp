/*
** EPITECH PROJECT, 2020
** CPP_zia_2019
** File description:
** Stylesheet.hpp
*/

#pragma once

namespace Stylesheet {
    constexpr inline auto default_style() noexcept
    {
        return R"eof(
<link rel='shortcut icon' type='image/png' href="favicon.png" />
<style>
body {
    font-family: monospace;
    font-size: 133%;
}

table {
    border-collapse: collapse;
    min-width: 50%;
}

table, th, td {
    border: 1px solid black;
}

th, td {
    padding: 5px;
}

th {
    background-color: #c0c0c0;
}

tr:hover {
    background-color: #f0f0f0;
}
</style>
)eof";
    }
}
