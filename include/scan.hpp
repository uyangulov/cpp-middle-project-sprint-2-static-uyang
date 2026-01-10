#pragma once

#include <cstddef>
#include <functional>
#include <tuple>
#include <type_traits>
#include <utility>

#include "format_string.hpp"
#include "parse.hpp"
#include "types.hpp"

namespace stdx {

template <details::format_string fmt, details::fixed_string source, typename... Ts, size_t... Is>
consteval details::scan_result<std::remove_cv_t<Ts>...>

scan_impl(std::index_sequence<Is...>) {
    return {std::make_tuple(stdx::details::parse_input<Is, fmt, source, std::remove_cv_t<Ts>>()...)};
}

template <details::format_string fmt, details::fixed_string source, typename... Ts>
consteval details::scan_result<std::remove_cv_t<Ts>...> scan() {
    return scan_impl<fmt, source, std::remove_cv_t<Ts>...>(std::index_sequence_for<Ts...>{});
}

}  // namespace stdx