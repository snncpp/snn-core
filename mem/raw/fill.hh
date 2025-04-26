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
    void fill(const not_null<T*> destination, const byte_size<usize> size,
              const byte value) noexcept
    {
        SNN_DIAGNOSTIC_PUSH
        SNN_DIAGNOSTIC_IGNORE_UNSAFE_BUFFER_USAGE_IN_LIBC_CALL

        __builtin_memset(destination.get(), value, size.get());

        SNN_DIAGNOSTIC_POP
    }

    template <>
    constexpr void fill<char>(const not_null<char*> destination, const byte_size<usize> size,
                              const byte value) noexcept
    {
        if (!std::is_constant_evaluated())
        {
            SNN_DIAGNOSTIC_PUSH
            SNN_DIAGNOSTIC_IGNORE_UNSAFE_BUFFER_USAGE_IN_LIBC_CALL

            __builtin_memset(destination.get(), value, size.get());

            SNN_DIAGNOSTIC_POP
        }
        else
        {
            for (usize i = 0; i < size.get(); ++i)
            {
                SNN_DIAGNOSTIC_PUSH
                SNN_DIAGNOSTIC_IGNORE_UNSAFE_BUFFER_USAGE

                destination.get()[i] = to_char(value);

                SNN_DIAGNOSTIC_POP
            }
        }
    }

    template <>
    constexpr void fill<byte>(const not_null<byte*> destination, const byte_size<usize> size,
                              const byte value) noexcept
    {
        if (!std::is_constant_evaluated())
        {
            SNN_DIAGNOSTIC_PUSH
            SNN_DIAGNOSTIC_IGNORE_UNSAFE_BUFFER_USAGE_IN_LIBC_CALL

            __builtin_memset(destination.get(), value, size.get());

            SNN_DIAGNOSTIC_POP
        }
        else
        {
            for (usize i = 0; i < size.get(); ++i)
            {
                SNN_DIAGNOSTIC_PUSH
                SNN_DIAGNOSTIC_IGNORE_UNSAFE_BUFFER_USAGE

                destination.get()[i] = value;

                SNN_DIAGNOSTIC_POP
            }
        }
    }
}
