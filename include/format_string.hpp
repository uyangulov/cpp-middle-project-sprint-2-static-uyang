#pragma once

#include <algorithm>
#include <array>
#include <cstddef>
#include <expected>
#include <iterator>
#include <utility>

#include "types.hpp"

namespace stdx::details {

template <fixed_string Str>
class format_string {
public:
    static constexpr auto fmt = Str;

    static consteval std::expected<size_t, parse_error> find_brace_num() {
        constexpr auto open_braces_count = std::count(std::begin(fmt.data), std::end(fmt.data), '{');
        constexpr auto close_braces_count = std::count(std::begin(fmt.data), std::end(fmt.data), '}');
        if constexpr (close_braces_count != open_braces_count)
            return std::unexpected<parse_error>{"Open brace count is not equal to closing brace count"};
        else
            return close_braces_count;
    }

    static consteval std::size_t compute_num_placeholders() {
        constexpr auto res = find_brace_num();
        if constexpr (!res)
            static_assert(false);
        return res.value();
    }

    static constexpr auto number_placeholders = compute_num_placeholders();

    using positionPairs = std::array<std::pair<size_t, size_t>, number_placeholders>;

    static consteval std::expected<positionPairs, parse_error> get_placeholder_positions() {
        constexpr size_t N = fmt.size();
        if (!N)
            return {};
        size_t placeholder_count = 0;
        size_t pos = 0;
        const size_t size = N - 1;  // -1 для игнорирования нуль-терминатора
        positionPairs pairs;

        while (pos < size) {
            // Пропускаем все символы до '{'
            if (fmt.data[pos] != '{') {
                ++pos;
                continue;
            } else {
                pairs[placeholder_count].first = pos;
            }

            // Проверяем незакрытый плейсхолдер
            if (pos + 1 >= size) {
                return std::unexpected(parse_error{"Unclosed last placeholder"});
            }

            // Начало плейсхолдера
            ++placeholder_count;
            ++pos;

            // Проверка спецификатора формата
            if (fmt.data[pos] == '%') {
                ++pos;
                if (pos >= size) {
                    return std::unexpected(parse_error{"Unclosed last placeholder"});
                }

                // Проверяем допустимые спецификаторы
                const char spec = fmt.data[pos];
                constexpr char valid_specs[] = {'d', 'u', 'f', 's'};
                bool valid = false;

                for (const char s : valid_specs) {
                    if (spec == s) {
                        valid = true;
                        break;
                    }
                }

                if (!valid) {
                    return std::unexpected(parse_error{"Invalid specifier."});
                }
                ++pos;
            }

            // Проверяем закрывающую скобку
            if (pos >= size || fmt.data[pos] != '}') {
                return std::unexpected(parse_error{"\'}\' hasn't been found in appropriate place"});
            } else {
                pairs[placeholder_count - 1].second = pos;
            }
            ++pos;
        }
        return pairs;
    }

    static consteval positionPairs compute_placeholder_positions() {
        constexpr auto res = get_placeholder_positions();
        if constexpr (!res)
            static_assert(false);
        return res.value();
    }

    static constexpr auto placeholder_positions = compute_placeholder_positions();
};

template <fixed_string f>
consteval auto operator""_fs() {
    return format_string<f>{};
}

}  // namespace stdx::details
