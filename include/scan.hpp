#pragma once

#include <tuple>

#include "format_string.hpp"
#include "parse.hpp"
#include "types.hpp"
#include <functional>

namespace stdx {

template <details::format_string fmt, details::fixed_string source, typename... Ts>
consteval details::scan_result<Ts...> scan() {
    details::scan_result<Ts...> out;
    auto handle_index = [&]<std::size_t I>() {
        using T = std::tuple_element_t<I, std::tuple<Ts...>>;
        auto res = stdx::details::parse_input<I, fmt, source, T>();
        std::get<I>(out.results) = *std::move(res);
    };

    std::invoke([&]<std::size_t... Is>(std::index_sequence<Is...>) { (handle_index.template operator()<Is>(), ...); },
                std::index_sequence_for<Ts...>{});

    return out;
}

}  // namespace stdx