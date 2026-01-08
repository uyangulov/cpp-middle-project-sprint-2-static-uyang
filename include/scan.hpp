#pragma once

#include <cstddef>
#include <functional>
#include <tuple>
#include <utility>

#include "format_string.hpp"
#include "parse.hpp"
#include "types.hpp"

namespace stdx
{

template <details::format_string fmt, details::fixed_string source, typename T1,
          typename T2>
consteval details::scan_result<T1, T2> scan()
{
    return details::scan_result<T1, T2>(
        std::make_tuple(*stdx::details::parse_input<0, fmt, source, T1>(),
                        *stdx::details::parse_input<1, fmt, source, T2>()));
}

template <details::format_string fmt, details::fixed_string source, typename T>
consteval details::scan_result<T> scan()
{
    return details::scan_result<T>(
        std::make_tuple(*stdx::details::parse_input<0, fmt, source, T>()));
}

// template <details::format_string fmt, details::fixed_string source,
//           typename... Ts>
// consteval details::scan_result<Ts...> scan()
// {
//     details::scan_result<T1, T2, T3>(std::)

//         return details::scan_result<Ts...>(std::make_tuple(Elements &&
//         args...))

//             return []<std::size_t... Is>(std::index_sequence<Is...>)
//             consteval
//     {
//         details::scan_result<Ts...> out{};

//         ((std::get<Is>(out.results) = stdx::details::parse_input<
//               Is, fmt, source, std::tuple_element_t<Is,
//               std::tuple<Ts...>>>()),
//          ...);

//         return out;
//     }(std::index_sequence_for<Ts...>{});
// }

}  // namespace stdx