#include "format_string.hpp"
#include "parse_integral.hpp"
#include "types.hpp"

static void check()
{
    using stdx::details::operator""_fs;
    constexpr auto fmt = "%d"_fs;
    constexpr auto val = *stdx::details::parse_value<int16_t, fmt.fmt, "123">();
    static_assert(val == 123);
}

static void check_neg()
{
    using stdx::details::operator""_fs;
    constexpr auto fmt = "%d"_fs;
    constexpr auto val =
        *stdx::details::parse_value<int16_t, fmt.fmt, "-123">();
    static_assert(val == -123);
}
