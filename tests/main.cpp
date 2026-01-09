#include "format_string.hpp"
#include "scan.hpp"
#include "types.hpp"
#include <cstdint>
#include <string_view>
#include <sys/types.h>
#include <type_traits>

using stdx::details::operator""_fs;

static void check_string_view() {
    constexpr auto res = stdx::scan<"{%s}"_fs, "blah", std::string_view>();
    constexpr auto s = std::get<0>(res.results);
    static_assert(s == "blah");
}

static void check_2unsigned() {
    constexpr auto format = "{%u}_{%u}"_fs;
    constexpr stdx::details::fixed_string in = "255_42";
    constexpr auto res = stdx::scan<format, in, uint8_t, uint8_t>();
    constexpr auto num1 = std::get<0>(res.results);
    constexpr auto num2 = std::get<1>(res.results);
    static_assert(num1 == 255);
    static_assert(num2 == 42);
}

static void check_signed_and_unsigned() {
    constexpr auto format = "{%d}____{%u}"_fs;
    constexpr stdx::details::fixed_string in = "-44____64";
    constexpr auto res = stdx::scan<format, in, int8_t, uint8_t>();
    constexpr auto num1 = std::get<0>(res.results);
    constexpr auto num2 = std::get<1>(res.results);
    static_assert(num1 == -44);
    static_assert(num2 == 64);
}

static void check_4() {
    constexpr auto format = "{%d}____{%u}_{%d}__{%s}"_fs;
    constexpr stdx::details::fixed_string in = "-44____0_-56__blah";
    constexpr auto res =
        stdx::scan<format, in, int8_t, volatile uint8_t, const volatile int64_t, const std::string_view>();

    constexpr auto num1 = std::get<0>(res.results);
    constexpr auto num2 = std::get<1>(res.results);
    constexpr auto num3 = std::get<2>(res.results);
    constexpr auto s4 = std::get<3>(res.results);

    static_assert(num1 == -44);
    static_assert(num2 == 0);
    static_assert(num3 == -56);
    static_assert(s4 == "blah");
}

static void check_uint16_max_and_zero() {
    constexpr auto format = "{%u}_{%u}"_fs;
    constexpr stdx::details::fixed_string input = "65535_0";
    constexpr auto res = stdx::scan<format, input, uint16_t, uint16_t>();
    static_assert(std::get<0>(res.results) == 65535u);
    static_assert(std::get<1>(res.results) == 0u);
}

static void check_int16_min_and_max() {
    constexpr auto format = "{%d}_{%d}"_fs;
    constexpr stdx::details::fixed_string input = "-32768_32767";
    constexpr auto res = stdx::scan<format, input, int16_t, int16_t>();
    static_assert(std::get<0>(res.results) == std::numeric_limits<int16_t>::min());
    static_assert(std::get<1>(res.results) == std::numeric_limits<int16_t>::max());
}

static void check_uint16_some_intermediate_value() {
    constexpr auto format = "{%u}_{%u}"_fs;
    constexpr stdx::details::fixed_string input = "2_1";
    constexpr auto res = stdx::scan<format, input, uint16_t, uint16_t>();
    static_assert(std::get<0>(res.results) == 2);
    static_assert(std::get<1>(res.results) == 1);
}

static void check_int16_some_intermediate_value() {
    constexpr auto format = "{%d}_{%d}"_fs;
    constexpr stdx::details::fixed_string input = "0_12345";
    constexpr auto res = stdx::scan<format, input, int16_t, int16_t>();
    static_assert(std::get<0>(res.results) == 0);
    static_assert(std::get<1>(res.results) == 12345);
}

static void check_uint32_max_and_zero() {
    constexpr auto format = "{%u}_{%u}"_fs;
    constexpr stdx::details::fixed_string input = "4294967295_0";
    constexpr auto res = stdx::scan<format, input, uint32_t, uint32_t>();
    static_assert(std::get<0>(res.results) == 4294967295u);
    static_assert(std::get<1>(res.results) == 0u);
}

static void check_int32_min_and_max() {
    constexpr auto format = "{%d}_{%d}"_fs;
    constexpr stdx::details::fixed_string input = "-2147483648_2147483647";
    constexpr auto res = stdx::scan<format, input, int32_t, int32_t>();
    static_assert(std::get<0>(res.results) == std::numeric_limits<int32_t>::min());
    static_assert(std::get<1>(res.results) == std::numeric_limits<int32_t>::max());
}

static void check_uint32_some_intermediate_value() {
    constexpr auto format = "{%u}_{%u}"_fs;
    constexpr stdx::details::fixed_string input = "2147483647_100000";
    constexpr auto res = stdx::scan<format, input, uint32_t, uint32_t>();
    static_assert(std::get<0>(res.results) == 2147483647u);
    static_assert(std::get<1>(res.results) == 100000u);
}

static void check_int32_some_intermediate_value() {
    constexpr auto format = "{%d}_{%d}"_fs;
    constexpr stdx::details::fixed_string input = "-100000_100000";
    constexpr auto res = stdx::scan<format, input, int32_t, int32_t>();
    static_assert(std::get<0>(res.results) == -100000);
    static_assert(std::get<1>(res.results) == 100000);
}

static void check_uint64_max_and_zero() {
    constexpr auto format = "{%u}_{%u}"_fs;
    constexpr stdx::details::fixed_string input = "18446744073709551615_0";
    constexpr auto res = stdx::scan<format, input, uint64_t, uint64_t>();
    static_assert(std::get<0>(res.results) == 18446744073709551615ULL);
    static_assert(std::get<1>(res.results) == 0ULL);
}

static void check_int64_min_and_max() {
    constexpr auto format = "{%d}_{%d}"_fs;
    constexpr stdx::details::fixed_string input = "-9223372036854775808_9223372036854775807";
    constexpr auto res = stdx::scan<format, input, int64_t, int64_t>();
    static_assert(std::get<0>(res.results) == std::numeric_limits<int64_t>::min());
    static_assert(std::get<1>(res.results) == std::numeric_limits<int64_t>::max());
}

static void check_uint64_some_intermediate_value() {
    constexpr auto format = "{%u}_foooo_{%u}"_fs;
    constexpr stdx::details::fixed_string input = "9223372036854775807_foooo_1234567890";
    constexpr auto res = stdx::scan<format, input, uint64_t, uint64_t>();
    static_assert(std::get<0>(res.results) == 9223372036854775807ULL);
    static_assert(std::get<1>(res.results) == 1234567890ULL);
}

static void check_int64_some_intermediate_value() {
    constexpr auto format = "{%d}_{%d}"_fs;
    constexpr stdx::details::fixed_string input = "-1234567890_1234567890";
    constexpr auto res = stdx::scan<format, input, int64_t, int64_t>();
    static_assert(std::get<0>(res.results) == -1234567890LL);
    static_assert(std::get<1>(res.results) == 1234567890LL);
}

int main() {
    check_uint16_max_and_zero();
    check_int16_min_and_max();
    check_uint16_some_intermediate_value();
    check_int16_some_intermediate_value();

    check_uint32_max_and_zero();
    check_int32_min_and_max();
    check_uint32_some_intermediate_value();
    check_int32_some_intermediate_value();

    check_uint64_max_and_zero();
    check_int64_min_and_max();
    check_uint64_some_intermediate_value();
    check_int64_some_intermediate_value();

    check_string_view();
    check_2unsigned();
    check_signed_and_unsigned();
    check_4();
}
