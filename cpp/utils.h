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

inline auto compute_z_array(const std::string_view str) -> std::vector<size_t>
{
    ADS_ASSERT(!str.empty(), "Cannot compute z-array for an empty string");

    std::vector<size_t> z_array(str.length(), 0);
    z_array[0] = str.length();

    for (size_t i = 1; i < str.length(); ++i)
    {
        if (str[i] == str[i - 1])
        {
            z_array[1]++;
        }
        else { break; }
    }

    size_t l = 0;
    size_t r = 0;

    if (z_array[1] > 0) { r = z_array[1], l = 1; }

    for (size_t k = 2; k < str.length(); ++k)
    {
        // case 1
        if (k > r)
        {
            // do explicit character comparison
            for (size_t i = k; i < str.length(); ++i)
            {
                if (str[i] == str[i - k])
                {
                    z_array[k]++;
                }
                else { break; }
            }
            r = k + z_array[k] - 1;
            l = k;
        }
        else
        {
            const size_t beta = r - k + 1;

            // case 2(a)
            if (auto const z_k_prime = z_array[k - l]; z_k_prime < beta)
            {
                z_array[k] = z_k_prime;
            }
            else
            {
                // case 2(b)
                size_t n_matched = 0;
                for (size_t i = r + 1; i < str.length(); ++i)
                {
                    if (str[i] == str[i - k])
                    {
                        n_matched++;
                    }
                    else { break; }
                }
                l = k;
                r = r + n_matched;
                z_array[k] = r - k + 1;
            }
        }
    }

    return z_array;
}

#endif //ADS_UTILS_H
