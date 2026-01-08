#include "format_string.hpp"
#include "scan.hpp"
#include "types.hpp"
#include <cstdint>
#include <sys/types.h>

using stdx::details::operator""_fs;

void check_unsigned()
{
    constexpr auto format = "{%u}"_fs;
    constexpr stdx::details::fixed_string in = "64";
    constexpr auto res = stdx::scan<format, in, uint8_t>();
    constexpr auto num = std::get<0>(res.results);
    static_assert(num == 64);
}

void check_2unsigned()
{
    constexpr auto format = "{%u}_{%u}"_fs;
    constexpr stdx::details::fixed_string in = "64_64";
    constexpr auto res = stdx::scan<format, in, uint8_t, uint8_t>();
    constexpr auto num1 = std::get<0>(res.results);
    constexpr auto num2 = std::get<1>(res.results);
    static_assert(num1 == 64);
    static_assert(num2 == 64);
}

void check_signed()
{
    constexpr auto format = "{%d}"_fs;
    constexpr stdx::details::fixed_string in = "-64";
    constexpr auto res = stdx::scan<format, in, int8_t>();
    constexpr auto num = std::get<0>(res.results);
    static_assert(num == -64);
}

void check_signed_and_unsigned()
{
    constexpr auto format = "{%d}____{%u}"_fs;
    constexpr stdx::details::fixed_string in = "-44____64";
    constexpr auto res = stdx::scan<format, in, int8_t, uint8_t>();
    constexpr auto num1 = std::get<0>(res.results);
    constexpr auto num2 = std::get<1>(res.results);
    static_assert(num1 == -44);
    static_assert(num2 == 64);
}

void check_triple()
{
    constexpr auto format = "{%d}____{%u}_{%d}"_fs;
    constexpr stdx::details::fixed_string in = "-44____64_-56";
    constexpr auto res = stdx::scan<format, in, int8_t, uint8_t, int8_t>();
    constexpr auto num1 = std::get<0>(res.results);
    constexpr auto num2 = std::get<1>(res.results);
    constexpr auto num3 = std::get<2>(res.results);
    static_assert(num1 == -44);
    static_assert(num2 == 64);
    static_assert(num3 == -56);
}

int main() {}
