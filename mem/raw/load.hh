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

    template <strict_unsigned_integral UInt, octet Octet>
    [[nodiscard]] constexpr UInt load(const Octet* const data) noexcept
    {
        if constexpr (std::is_same_v<UInt, u16>)
        {
            u16 i = 0;
            static_assert(sizeof(i) == 2);
            if (std::is_constant_evaluated())
            {
                SNN_DIAGNOSTIC_PUSH
                SNN_DIAGNOSTIC_IGNORE_UNSAFE_BUFFER_USAGE

                i = static_cast<byte>(data[1]);
                i <<= 8u;
                i |= u16{static_cast<byte>(data[0])};

                SNN_DIAGNOSTIC_POP
            }
            else
            {
                SNN_DIAGNOSTIC_PUSH
                SNN_DIAGNOSTIC_IGNORE_UNSAFE_BUFFER_USAGE_IN_LIBC_CALL

                __builtin_memcpy(&i, data, sizeof(i));

                SNN_DIAGNOSTIC_POP
            }
            return i;
        }
        else if constexpr (std::is_same_v<UInt, u32>)
        {
            u32 i = 0;
            static_assert(sizeof(i) == 4);
            if (std::is_constant_evaluated())
            {
                SNN_DIAGNOSTIC_PUSH
                SNN_DIAGNOSTIC_IGNORE_UNSAFE_BUFFER_USAGE

                i = static_cast<byte>(data[0]);
                i |= u32{static_cast<byte>(data[1])} << 8u;
                i |= u32{static_cast<byte>(data[2])} << 16u;
                i |= u32{static_cast<byte>(data[3])} << 24u;

                SNN_DIAGNOSTIC_POP
            }
            else
            {
                SNN_DIAGNOSTIC_PUSH
                SNN_DIAGNOSTIC_IGNORE_UNSAFE_BUFFER_USAGE_IN_LIBC_CALL

                __builtin_memcpy(&i, data, sizeof(i));

                SNN_DIAGNOSTIC_POP
            }
            return i;
        }
        else if constexpr (std::is_same_v<UInt, u64>)
        {
            u64 i = 0;
            static_assert(sizeof(i) == 8);
            if (std::is_constant_evaluated())
            {
                SNN_DIAGNOSTIC_PUSH
                SNN_DIAGNOSTIC_IGNORE_UNSAFE_BUFFER_USAGE

                i = static_cast<byte>(data[0]);
                i |= u64{static_cast<byte>(data[1])} << 8u;
                i |= u64{static_cast<byte>(data[2])} << 16u;
                i |= u64{static_cast<byte>(data[3])} << 24u;
                i |= u64{static_cast<byte>(data[4])} << 32u;
                i |= u64{static_cast<byte>(data[5])} << 40u;
                i |= u64{static_cast<byte>(data[6])} << 48u;
                i |= u64{static_cast<byte>(data[7])} << 56u;

                SNN_DIAGNOSTIC_POP
            }
            else
            {
                SNN_DIAGNOSTIC_PUSH
                SNN_DIAGNOSTIC_IGNORE_UNSAFE_BUFFER_USAGE_IN_LIBC_CALL

                __builtin_memcpy(&i, data, sizeof(i));

                SNN_DIAGNOSTIC_POP
            }
            return i;
        }
        else
        {
            static_assert(meta::always_false<UInt>, "Not implemented.");
            return 0;
        }
    }
}
