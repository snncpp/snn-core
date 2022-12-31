// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

// # Load (word)

#pragma once

#include "snn-core/core.hh"

namespace snn::mem::raw
{
    // ## Functions

    // ### load

    // Load unsigned integral from _octet_ pointer.

    template <typename Int, octet Octet>
    [[nodiscard]] constexpr Int load(const Octet* const data) noexcept
    {
        if constexpr (std::is_same_v<Int, u16>)
        {
            u16 i = 0;
            static_assert(sizeof(i) == 2);
            if (std::is_constant_evaluated())
            {
                i = static_cast<byte>(data[1]);
                i <<= 8u;
                i |= u16{static_cast<byte>(data[0])};
            }
            else
            {
                __builtin_memcpy(&i, data, sizeof(i));
            }
            return i;
        }
        else if constexpr (std::is_same_v<Int, u32>)
        {
            u32 i = 0;
            static_assert(sizeof(i) == 4);
            if (std::is_constant_evaluated())
            {
                i = static_cast<byte>(data[0]);
                i |= u32{static_cast<byte>(data[1])} << 8u;
                i |= u32{static_cast<byte>(data[2])} << 16u;
                i |= u32{static_cast<byte>(data[3])} << 24u;
            }
            else
            {
                __builtin_memcpy(&i, data, sizeof(i));
            }
            return i;
        }
        else if constexpr (std::is_same_v<Int, u64>)
        {
            u64 i = 0;
            static_assert(sizeof(i) == 8);
            if (std::is_constant_evaluated())
            {
                i = static_cast<byte>(data[0]);
                i |= u64{static_cast<byte>(data[1])} << 8u;
                i |= u64{static_cast<byte>(data[2])} << 16u;
                i |= u64{static_cast<byte>(data[3])} << 24u;
                i |= u64{static_cast<byte>(data[4])} << 32u;
                i |= u64{static_cast<byte>(data[5])} << 40u;
                i |= u64{static_cast<byte>(data[6])} << 48u;
                i |= u64{static_cast<byte>(data[7])} << 56u;
            }
            else
            {
                __builtin_memcpy(&i, data, sizeof(i));
            }
            return i;
        }
        else
        {
            static_assert(meta::always_false<Int>, "Not implemented.");
            return 0;
        }
    }
}
