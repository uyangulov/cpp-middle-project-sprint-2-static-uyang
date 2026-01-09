#include "types.hpp"
#include <charconv>
#include <concepts>
#include <expected>
#include <sys/types.h>
#include <system_error>

namespace stdx::details {

template <char C, fixed_string F>
consteval void check_spec() {
    static_assert(F.size() == 2, "Wrong specifier size. Expected 2");
    static_assert(F.data[0] == '%', "Expected '%' at position 0 of specifier");
    static_assert(F.data[1] == C, "Wrong character at position 1 of specifier");
}

template <std::integral V, fixed_string S>
consteval std::expected<V, parse_error> parse_digits() {
    static_assert(S.size() > 0, "Empty string");
    V val{};
    auto [ptr, ec] = std::from_chars(S.data, S.data + S.size(), val);
    if (ec == std::errc{})
        return val;
    else if (ec == std::errc::invalid_argument)
        return std::unexpected<parse_error>("Not a number");
    else if (ec == std::errc::result_out_of_range)
        return std::unexpected<parse_error>("Value too large for requested type");
    else
        return std::unexpected<parse_error>("Unknown error");
}

template <std::unsigned_integral U, fixed_string F, fixed_string S>
consteval U parse_value() {
    check_spec<'u', F>();
    constexpr auto res = parse_digits<U, S>();
    static_assert(res.has_value(), "Parsing failed");
    return res.value();
};

template <std::signed_integral I, fixed_string F, fixed_string S>
consteval I parse_value() {
    check_spec<'d', F>();
    constexpr auto res = parse_digits<I, S>();
    static_assert(res.has_value(), "Parsing failed");
    return res.value();
};

template <std::same_as<std::string_view> SV, fixed_string F, fixed_string S>
consteval SV parse_value() {
    check_spec<'s', F>();
    constexpr std::size_t n = S.size() ? S.size() : 0;
    return SV{S.data, n};
}

}  // namespace stdx::details