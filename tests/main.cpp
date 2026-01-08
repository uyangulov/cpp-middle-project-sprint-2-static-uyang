#include "format_string.hpp"
#include "scan.hpp"
#include "types.hpp"
#include <cstdint>

int main()
{
    constexpr stdx::details::fixed_string in = "42";
    constexpr stdx::details::fixed_string fmt = "{%u}";
    constexpr stdx::details::format_string<fmt> format = {};
    constexpr auto res = stdx::scan<format, in, uint8_t>();
    constexpr auto num = std::get<0>(res.results);
    static_assert(num == 42);
}
