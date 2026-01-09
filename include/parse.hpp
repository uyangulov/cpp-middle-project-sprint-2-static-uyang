#pragma once

#include "parse_integral.hpp"
#include "parse_string.hpp"

#include <cstddef>
#include <expected>
#include <string_view>
#include <utility>

#include "format_string.hpp"
#include "types.hpp"

namespace stdx::details {

template <int I, format_string fmt, fixed_string source>
consteval auto get_current_source_for_parsing() {
    static_assert(I >= 0 && I < fmt.number_placeholders, "Invalid placeholder index");

    constexpr auto to_sv = [](const auto &fs) { return std::string_view(fs.data, fs.size() - 1); };

    constexpr auto fmt_sv = to_sv(fmt.fmt);
    constexpr auto src_sv = to_sv(source);
    constexpr auto &positions = fmt.placeholder_positions;

    // Получаем границы текущего плейсхолдера в формате
    constexpr auto pos_i = positions[I];
    constexpr size_t fmt_start = pos_i.first, fmt_end = pos_i.second;

    // Находим начало в исходной строке
    constexpr auto src_start = [&] {
        if constexpr (I == 0) {
            return fmt_start;
        } else {
            // Находим конец предыдущего плейсхолдера в исходной строке
            constexpr auto prev_bounds = get_current_source_for_parsing<I - 1, fmt, source>();
            const auto prev_end = prev_bounds.second;

            // Получаем разделитель между текущим и предыдущим плейсхолдерами
            constexpr auto prev_fmt_end = positions[I - 1].second;
            constexpr auto sep = fmt_sv.substr(prev_fmt_end + 1, fmt_start - (prev_fmt_end + 1));

            // Ищем разделитель после предыдущего значения
            auto pos = src_sv.find(sep, prev_end);
            return pos != std::string_view::npos ? pos + sep.size() : src_sv.size();
        }
    }();

    // Находим конец в исходной строке
    constexpr auto src_end = [&] {
        // Получаем разделитель после текущего плейсхолдера
        if constexpr (fmt_end == (fmt_sv.size() - 1)) {
            return src_sv.size();
        }
        constexpr auto sep =
            fmt_sv.substr(fmt_end + 1, (I < fmt.number_placeholders - 1) ? positions[I + 1].first - (fmt_end + 1)
                                                                         : fmt_sv.size() - (fmt_end + 1));

        // Ищем разделитель после текущего значения
        constexpr auto pos = src_sv.find(sep, src_start);
        return pos != std::string_view::npos ? pos : src_sv.size();
    }();
    return std::pair{src_start, src_end};
}

template <int I, format_string fmt, fixed_string source, typename T>
consteval std::expected<T, parse_error> parse_input() {
    constexpr auto src = get_current_source_for_parsing<I, fmt, source>();
    constexpr auto first = src.first;
    constexpr auto second = src.second;
    constexpr std::size_t len = static_cast<std::size_t>(second - first);
    constexpr fixed_string<len> str{source.data + first, source.data + second};
    constexpr auto &positions = fmt.placeholder_positions;

    constexpr auto pos_i = positions[I];
    constexpr auto first_i = pos_i.first;
    constexpr auto second_i = pos_i.second;
    constexpr std::size_t len_i = static_cast<std::size_t>(second_i - first_i);

    constexpr fixed_string<len_i - 1> str_i{fmt.fmt.data + first_i + 1, fmt.fmt.data + second_i};

    constexpr auto res = parse_value<T, str_i, str>();
    if constexpr (!res.has_value())
        return std::unexpected<parse_error>{"parse_value failed"};

    return res;
}

}  // namespace stdx::details
