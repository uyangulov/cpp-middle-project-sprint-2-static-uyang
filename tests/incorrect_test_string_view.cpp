#include "format_string.hpp"
#include "scan.hpp"
#include "types.hpp"
#include <string_view>
#include <sys/types.h>

using stdx::details::operator""_fs;

static void check_incorrect_specifier()
{
    constexpr auto res = stdx::scan<"{%d}"_fs, "blah", std::string_view>();
}

static void check_ref_type()
{
    constexpr auto res = stdx::scan<"{%d}"_fs, "blah", std::string_view &>();
}