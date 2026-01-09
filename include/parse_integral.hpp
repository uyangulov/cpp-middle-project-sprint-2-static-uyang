#include "types.hpp"
#include <charconv>
#include <concepts>
#include <expected>
#include <sys/types.h>
#include <system_error>

namespace stdx::details {

template <std::integral V, fixed_string S>
consteval std::expected<V, parse_error> parse_value_impl() {
    V val{};
    auto [ptr, ec] = std::from_chars(S.data, S.data + S.size(), val);
    if (ec == std::errc{})
        return val;
    else if (ec == std::errc::invalid_argument)
        return std::unexpected<parse_error>("Not a number");
    else if (ec == std::errc::result_out_of_range)
        return std::unexpected<parse_error>("Value too large for requested type");
    else
        return std::unexpected<parse_error>("Uknown error");
}

template <std::unsigned_integral U, fixed_string F, fixed_string S>
consteval std::expected<U, parse_error> parse_value() {
    static_assert(F.size() == 2, "Wrong specifier size. Expected 2");
    static_assert(F.data[0] == '%', "Expected '%' at position 0 of specifier");
    static_assert(F.data[1] == 'u', "Expected 'u' at position 1 of specifier");
    static_assert(S.size() > 0, "Empty string");
    return parse_value_impl<U, S>();
};

template <std::signed_integral I, fixed_string F, fixed_string S>
consteval std::expected<I, parse_error> parse_value() {
    static_assert(F.size() == 2, "Wrong specifier size. Expected 2");
    static_assert(F.data[0] == '%', "Expected '%' at position 0 of specifier");
    static_assert(F.data[1] == 'd', "Expected 'u' at position 1 of specifier");
    static_assert(S.size() > 0, "Empty string");
    return parse_value_impl<I, S>();
};

}  // namespace stdx::details