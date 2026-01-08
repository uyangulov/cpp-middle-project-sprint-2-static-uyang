#include "format_string.hpp"
#include "scan.hpp"
#include "types.hpp"
#include <array>
#include <cstddef>
#include <cstdint>
#include <sys/types.h>
#include <utility>

using stdx::details::operator""_fs;

void check_single()
{
    constexpr auto format_u = "{%u}"_fs;
    constexpr auto format_d = "{%d}"_fs;
    constexpr auto format_f = "{%f}"_fs;
    constexpr auto format_s = "{%s}"_fs;

    constexpr size_t correct_num = 1;
    constexpr std::array<std::pair<size_t, size_t>, correct_num> correct_pairs =
        {std::make_pair(0, 3)};

    static_assert(format_u.compute_num_placeholders() == correct_num);
    static_assert(format_d.compute_num_placeholders() == correct_num);
    static_assert(format_f.compute_num_placeholders() == correct_num);
    static_assert(format_s.compute_num_placeholders() == correct_num);

    static_assert(format_u.get_placeholder_positions() == correct_pairs);
    static_assert(format_d.get_placeholder_positions() == correct_pairs);
    static_assert(format_f.get_placeholder_positions() == correct_pairs);
    static_assert(format_s.get_placeholder_positions() == correct_pairs);
}

void check_double()
{
    constexpr auto format = "{%u}_some_text_{%d}"_fs;

    constexpr size_t correct_num = 2;
    constexpr std::array<std::pair<size_t, size_t>, correct_num> correct_pairs =
        {std::make_pair(0, 3), {15, 18}};

    static_assert(format.compute_num_placeholders() == correct_num);
    static_assert(format.get_placeholder_positions() == correct_pairs);
}

void check_multiple()
{
    constexpr auto format = "{%u}_some_text_{%d}__more_text_{%f}_{%s}"_fs;

    constexpr size_t correct_num = 4;
    constexpr std::array<std::pair<size_t, size_t>, correct_num> correct_pairs =
        {
            std::make_pair(0, 3),    // {%u}
            std::make_pair(15, 18),  // {%d}
            std::make_pair(31, 34),  // {%f}
            std::make_pair(36, 39)   // {%s}
        };

    static_assert(format.compute_num_placeholders() == correct_num);
    static_assert(format.get_placeholder_positions() == correct_pairs);
}
