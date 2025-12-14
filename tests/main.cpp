#include "format_string.hpp"
#include "scan.hpp"
#include "types.hpp"
#include <cstdint>

int main() {
    /**
    / У МЕНЯ НЕ ПОЛУЧИЛОСЬ ДОВЕСТИ ДО КОНЦА.

    Несмотря на то, что я в функциях get_placeholder_positions и parse_value
    возвращаю значения, заведомо правильные для теста, приведенного ниже, ничего не работает.
    (Понятно, что и другие тесты не будут тоже)

    Как мне кажется, я исчерпал идеи к тому, что бы я мог поправить. Поэтому need help.
    */
    constexpr stdx::details::fixed_string in = "42";
    constexpr stdx::details::fixed_string fmt = "{%u}";
    constexpr stdx::details::format_string<fmt> format = {};
    constexpr auto res = stdx::scan<format, in, uint8_t>();
    constexpr auto num = std::get<0>(res.results);
    static_assert(num == 42);
}
