//
// Created by rasheed on 2026/11/09.
//

#ifndef ADS_UTILS_H
#define ADS_UTILS_H
#include <iostream>
#include <random>
#include <source_location>
#include <string_view>

inline auto
ads_assert(
    const bool condition,
    const std::string_view message,
    const std::source_location& location = std::source_location::current()
) -> void
{
    if (not condition) [[unlikely]]
    {
        std::println(std::cerr, "{}:{} ({}): {}", location.file_name(), location.line(), location.function_name(),
                     message);
        std::exit(1);
    }
}

#define ADS_ASSERT(condition, message) ads_assert(condition, message)

inline auto get_random_from_str(const std::string_view& str) -> char
{
    ADS_ASSERT(!str.empty(), "cannot generate random string from an empty string");
    std::mt19937 generator(std::random_device{}());
    std::uniform_int_distribution<size_t> idx(0, str.size() - 1);

    return str[idx(generator)];
}

inline auto strip_whitespace(std::string_view str_v) -> std::string_view
{
    constexpr std::string_view whitespace = " \r\n\t\f\v";

    if (const auto start = str_v.find_first_not_of(whitespace); start != std::string_view::npos)
    {
        str_v.remove_prefix(start);
    }
    else { return ""; }

    if (const auto end = str_v.find_last_not_of(whitespace); end != std::string_view::npos)
    {
        str_v.remove_suffix(str_v.size() - 1 - end);
    }

    return str_v;
}

inline auto phred33_to_q(unsigned char const qual) -> int
{
    return qual - 33;
}

#endif //ADS_UTILS_H
