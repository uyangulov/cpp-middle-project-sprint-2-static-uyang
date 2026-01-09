#include "format_string.hpp"
#include "scan.hpp"
#include "types.hpp"
#include <cstdint>
#include <sys/types.h>

using stdx::details::operator""_fs;

static void check_pos_overflow() { constexpr auto res = stdx::scan<"{%d}"_fs, "128", int8_t>(); }

static void check_neg_overflow() { constexpr auto res = stdx::scan<"{%d}"_fs, "-129", int8_t>(); }

static void check_incorrect_specifier() { constexpr auto res = stdx::scan<"{%u}"_fs, "1", int8_t>(); }

static void check_incorrect_type() { constexpr auto res = stdx::scan<"{%u}"_fs, "1", std::string>(); }

static void check_empty_string() { constexpr auto res = stdx::scan<"{%d}"_fs, "", int8_t>(); }

static void check_garbage() { constexpr auto res = stdx::scan<"{%d}"_fs, "asdfgb", int8_t>(); }