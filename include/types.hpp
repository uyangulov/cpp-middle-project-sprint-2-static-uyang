#pragma once
#include <cstddef>
#include <cstring>
#include <stdint.h>
#include <tuple>

namespace stdx::details
{

// Шаблонный класс, хранящий C-style строку фиксированной длины

template <size_t N>
struct fixed_string
{

    static consteval size_t size() { return N; }

    constexpr fixed_string(const char (&arr)[N]) noexcept
    {
        for (std::size_t i = 0; i < N; ++i)
        {
            data[i] = arr[i];
        }
    }

    template <size_t N_new>
    constexpr fixed_string(const char (&arr)[N_new]) noexcept
    {
        static_assert(N_new <= N, "input array too big");
        for (std::size_t i = 0; i < N_new; ++i)
        {
            data[i] = arr[i];
        }
    }

    constexpr fixed_string(const char *begin, const char *end) noexcept
    {
        auto dist = end - begin;
        for (std::size_t i = 0; i < static_cast<std::size_t>(dist); ++i)
            data[i] = begin[i];
    }

    char data[N];
};

struct parse_error : fixed_string<100>
{
};

template <typename... Ts>
struct scan_result
{
    std::tuple<Ts...> results;
};

}  // namespace stdx::details
