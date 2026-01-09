#include "types.hpp"
#include <concepts>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <expected>
#include <limits>
#include <sys/types.h>

namespace stdx::details
{

/**
Parse decimal string representation `source` to uint64_t

`source` must contain nothing but digits 0-9 (or static assert will prevent
compilation)

the decimal must fit uint64_t (or static assert will prevent
compilation)
**/
template <size_t N, size_t I, fixed_string source>
consteval uint64_t parse_digits_impl()
{
    static_assert(I >= 0, "Invalid index");
    static_assert(I < source.size(), "Invalid index");

    constexpr auto c = source.data[I];
    static_assert(c != '\0',
                  "Null terminator encountered while parsing digits");
    static_assert(c >= '0' && c <= '9', "Not a digit");

    constexpr auto val = static_cast<uint64_t>(c - '0');
    constexpr uint64_t m = std::numeric_limits<uint64_t>::max();

    if constexpr (I == 0)
        return val;
    else
    {
        constexpr auto prev_num = parse_digits_impl<N, I - 1, source>();
        constexpr auto thresh = static_cast<uint64_t>((m - val) / 10);
        if constexpr (prev_num > thresh)
            static_assert(false, "Overflow encountered while parsing");
        else
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
    static_assert(S.size() > 0, "Empty string");
    constexpr uint64_t val = parse_digits<S>();

    constexpr auto maxval =
        static_cast<uint64_t>(std::numeric_limits<U>::max());

    if constexpr (val > maxval)
        return std::unexpected<parse_error>(
            "Value too large for requested type");
    else
        return static_cast<U>(val);
}

template <std::signed_integral I, fixed_string F, fixed_string S>
consteval std::expected<I, parse_error> parse_value()
{
    // static_assert(F.size() == 3, "Wrong format specifier size (expected 2)");
    static_assert(F.data[0] == '%', "Expected '%' at position 0 of specifier");
    static_assert(F.data[1] == 'd', "Expected 'd' at position 1 of specifier");
    static_assert(S.size() > 0, "Empty string");
    // static_assert(F.data[2] == '\0', "Expected 'd' at position 1 of
    // specifier");

    if constexpr (S.data[0] == '-')
    {
        constexpr fixed_string<S.size() - 1> str{S.data + 1, S.data + S.size()};
        constexpr uint64_t val = parse_digits<str>();

        // hack for 2s complement
        constexpr auto abs_min =
            static_cast<uint64_t>(std::numeric_limits<I>::max()) + 1;

        // absolute value too big e.g. -1000 for int8_t
        if constexpr (val > abs_min)
            return std::unexpected<parse_error>(
                "Value too large for requested type");

        // edge case e.g. -128 for int8_t
        if constexpr (val == abs_min)
            return std::numeric_limits<I>::min();

        return -static_cast<I>(val);
    }
    else
    {
        constexpr uint64_t val = parse_digits<S>();
        constexpr auto maxval =
            static_cast<uint64_t>(std::numeric_limits<I>::max());
        if constexpr (val > maxval)
            return std::unexpected<parse_error>(
                "Value too large for requested type");
        return static_cast<I>(val);
    }
}

}  // namespace stdx::details