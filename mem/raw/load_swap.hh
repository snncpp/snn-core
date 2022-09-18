// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

// # Load-swap (word)

#pragma once

#include "snn-core/mem/raw/load.hh"

namespace snn::mem::raw
{
    // ## Functions

    // ### load_swap - From char pointer

    template <typename Int>
    [[nodiscard]] constexpr Int load_swap(const char*) noexcept
    {
        static_assert(meta::always_false<Int>, "Not implemented.");
        return 0;
    }

    template <>
    [[nodiscard]] constexpr u16 load_swap<u16>(const char* const data) noexcept
    {
        return __builtin_bswap16(mem::raw::load<u16>(data));
    }

    template <>
    [[nodiscard]] constexpr u32 load_swap<u32>(const char* const data) noexcept
    {
        return __builtin_bswap32(mem::raw::load<u32>(data));
    }

    template <>
    [[nodiscard]] constexpr u64 load_swap<u64>(const char* const data) noexcept
    {
        return __builtin_bswap64(mem::raw::load<u64>(data));
    }
}
