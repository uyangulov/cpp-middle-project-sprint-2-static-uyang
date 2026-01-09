#include "format_string.hpp"
#include "scan.hpp"
#include <cstdint>
#include <sys/types.h>

using stdx::details::operator""_fs;

static void check_incorrect_specifier() { constexpr auto res = stdx::scan<"{%s}"_fs, "1", uint8_t>(); }

static void check_reference_type() { constexpr auto res = stdx::scan<"{%s}"_fs, "1", uint8_t &>(); }

static void check_pos_overflow() { constexpr auto res = stdx::scan<"{%u}"_fs, "256", uint8_t>(); }

static void check_minus_zero() { constexpr auto res = stdx::scan<"{%u}"_fs, "-0", uint8_t>(); }

static void check_neg_overflow() { constexpr auto res = stdx::scan<"{%u}"_fs, "-1", uint8_t>(); }

static void check_empty_string() { constexpr auto res = stdx::scan<"{%u}"_fs, "", uint8_t>(); }

static void check_garbage() { constexpr auto res = stdx::scan<"{%u}"_fs, "asdfgb", uint8_t>(); }