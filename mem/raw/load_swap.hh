// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

// # Load-swap (word)

#pragma once

#include "snn-core/mem/raw/load.hh"

namespace snn::mem::raw
{
    // ## Functions

    // ### load_swap

    // Load (and swap) unsigned integral from _octet_ pointer.

    template <typename Int, octet Octet>
    [[nodiscard]] constexpr Int load_swap(const Octet* const data) noexcept
    {
        if constexpr (std::is_same_v<Int, u16>)
        {
            return __builtin_bswap16(mem::raw::load<u16>(data));
        }
        else if constexpr (std::is_same_v<Int, u32>)
        {
            return __builtin_bswap32(mem::raw::load<u32>(data));
        }
        else if constexpr (std::is_same_v<Int, u64>)
        {
            return __builtin_bswap64(mem::raw::load<u64>(data));
        }
        else
        {
            static_assert(meta::always_false<Int>, "Not implemented.");
            return 0;
        }
    }
}
