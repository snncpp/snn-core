// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

// # Fill (memset)

#pragma once

#include "snn-core/core.hh"

namespace snn::mem::raw
{
    // ## Functions

    // ### fill

    template <typename T>
    void fill(const not_null<T*> destination, const byte_size size, const byte value) noexcept
    {
        __builtin_memset(destination.get(), value, size.get());
    }

    template <>
    constexpr void fill<char>(const not_null<char*> destination, const byte_size size,
                              const byte value) noexcept
    {
        if (!std::is_constant_evaluated())
        {
            __builtin_memset(destination.get(), value, size.get());
        }
        else
        {
            for (usize i = 0; i < size.get(); ++i)
            {
                destination.get()[i] = to_char(value);
            }
        }
    }

    template <>
    constexpr void fill<byte>(const not_null<byte*> destination, const byte_size size,
                              const byte value) noexcept
    {
        if (!std::is_constant_evaluated())
        {
            __builtin_memset(destination.get(), value, size.get());
        }
        else
        {
            for (usize i = 0; i < size.get(); ++i)
            {
                destination.get()[i] = value;
            }
        }
    }
}
