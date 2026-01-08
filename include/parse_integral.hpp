#include <charconv>
#include <concepts>
#include <cstddef>
#include <expected>
#include <functional>
#include <optional>
#include <ratio>
#include <string_view>
#include <system_error>
#include <utility>

#include "format_string.hpp"
#include "types.hpp"

namespace stdx::details
{

template <size_t N, size_t I, fixed_string source>
consteval auto parse_digits_impl()
{
    static_assert(I >= 0, "Invalid index");
    static_assert(I < N, "Invalid index");

    constexpr auto c = source.data[I];
    static_assert(c != '\0', "Null terminator encountered");
    static_assert(c >= '0' && c <= '9', "Not a digit");

    constexpr auto val = static_cast<unsigned>(c - '0');

    if constexpr (I == 0)
        return val;
    else
    {
        constexpr auto prev_num = parse_digits_impl<N, I - 1, source>();
        return 10 * prev_num + val;
    }
}

template <fixed_string src>
consteval auto parse_digits()
{
    static_assert(src.size() > 0, "Empty string");

    constexpr size_t len =
        (src.data[src.size() - 1] == '\0') ? src.size() - 1 : src.size();

    return parse_digits_impl<len, len - 1, src>();
}

template <std::unsigned_integral U, fixed_string F, fixed_string S>
consteval std::expected<U, parse_error> parse_value()
{
    static_assert(F.data[0] == '%', "Expected '%' at position 0 of specifier");
    static_assert(F.data[1] == 'u', "Expected 'u' at position 1 of specifier");
    return parse_digits<S>();
}

template <std::signed_integral I, fixed_string F, fixed_string S>
consteval std::expected<I, parse_error> parse_value()
{
    // static_assert(F.size() == 3, "Wrong format specifier size (expected 2)");
    static_assert(F.data[0] == '%', "Expected '%' at position 0 of specifier");
    static_assert(F.data[1] == 'd', "Expected 'd' at position 1 of specifier");
    // static_assert(F.data[2] == '\0', "Expected 'd' at position 1 of
    // specifier");

    if constexpr (S.data[0] == '-')
    {
        constexpr fixed_string<S.size() - 1> str{S.data + 1, S.data + S.size()};

        return -static_cast<I>(parse_digits<str>());
    }
    else
    {
        return static_cast<I>(parse_digits<S>());
    }
}

}  // namespace stdx::details