// Copyright (c) 2024 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

// # Common bits functions

#pragma once

#include "snn-core/core.hh"

namespace snn::math::bits
{
    // ## Functions

    // ### reverse_bytes

    // Reverse the bytes in an unsigned integral.

    template <strict_unsigned_integral UInt>
    [[nodiscard]] constexpr UInt reverse_bytes(const UInt u) noexcept
    {
        if constexpr (std::is_same_v<UInt, u16>)
        {
            return __builtin_bswap16(u);
        }
        else if constexpr (std::is_same_v<UInt, u32>)
        {
            return __builtin_bswap32(u);
        }
        else if constexpr (std::is_same_v<UInt, u64>)
        {
            return __builtin_bswap64(u);
        }
        else
        {
            static_assert(meta::always_false<UInt>, "Not implemented.");
            return 0;
        }
    }
}
