#include "parse_integral.hpp"

static_assert(stdx::details::parse_digits<"5643">() == 5643);
static_assert(stdx::details::parse_digits<"645">() == 645);
static_assert(stdx::details::parse_digits<"123">() == 123);
static_assert(stdx::details::parse_digits<"12">() == 12);
static_assert(stdx::details::parse_digits<"0">() == 0);
