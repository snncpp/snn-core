// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

// # Load (word)

#pragma once

#include "snn-core/core.hh"

namespace snn::mem::raw
{
    // ## Functions

    // ### load - From char pointer

    template <typename Int>
    [[nodiscard]] constexpr Int load(const char*) noexcept
    {
        static_assert(meta::always_false<Int>, "Not implemented.");
        return 0;
    }

    template <>
    [[nodiscard]] constexpr u16 load<u16>(const char* const data) noexcept
    {
        u16 i = 0;
        static_assert(sizeof(i) == 2);
        if (std::is_constant_evaluated())
        {
            i = to_byte(data[1]);
            i <<= 8;
            i |= u16{to_byte(data[0])};
        }
        else
        {
            __builtin_memcpy(&i, data, sizeof(i));
        }
        return i;
    }

    template <>
    [[nodiscard]] constexpr u32 load<u32>(const char* const data) noexcept
    {
        u32 i = 0;
        static_assert(sizeof(i) == 4);
        if (std::is_constant_evaluated())
        {
            i = to_byte(data[0]);
            i |= u32{to_byte(data[1])} << 8;
            i |= u32{to_byte(data[2])} << 16;
            i |= u32{to_byte(data[3])} << 24;
        }
        else
        {
            __builtin_memcpy(&i, data, sizeof(i));
        }
        return i;
    }

    template <>
    [[nodiscard]] constexpr u64 load<u64>(const char* const data) noexcept
    {
        u64 i = 0;
        static_assert(sizeof(i) == 8);
        if (std::is_constant_evaluated())
        {
            i = to_byte(data[0]);
            i |= u64{to_byte(data[1])} << 8;
            i |= u64{to_byte(data[2])} << 16;
            i |= u64{to_byte(data[3])} << 24;
            i |= u64{to_byte(data[4])} << 32;
            i |= u64{to_byte(data[5])} << 40;
            i |= u64{to_byte(data[6])} << 48;
            i |= u64{to_byte(data[7])} << 56;
        }
        else
        {
            __builtin_memcpy(&i, data, sizeof(i));
        }
        return i;
    }
}
