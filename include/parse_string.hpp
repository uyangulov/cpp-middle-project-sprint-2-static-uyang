#include "types.hpp"
#include <concepts>
#include <cstddef>
#include <expected>
#include <string_view>

namespace stdx::details
{

template <typename T>
concept string_view_like =
    std::same_as<std::remove_cvref_t<T>, std::string_view>;

template <string_view_like SV, fixed_string F, fixed_string S>
consteval std::expected<SV, parse_error> parse_value()
{
    static_assert(F.data[0] == '%', "Expected '%' at position 0 of specifier");
    static_assert(F.data[1] == 's', "Expected 's' at position 1 of specifier");
    constexpr std::size_t n = S.size() ? S.size() : 0;
    return SV{S.data, n};
}

}  // namespace stdx::details