// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

// # Is equal (memcmp)

#pragma once

#include "snn-core/core.hh"

namespace snn::mem::raw
{
    // ## Functions

    // ### is_equal

    template <typename A, typename B>
    [[nodiscard]] constexpr bool is_equal(const not_null<A*> a, const not_null<B*> b,
                                          const byte_size<usize> size) noexcept
    {
        SNN_DIAGNOSTIC_PUSH
        SNN_DIAGNOSTIC_IGNORE_UNSAFE_BUFFER_USAGE_IN_LIBC_CALL

        return __builtin_memcmp(a.get(), b.get(), size.get()) == 0;

        SNN_DIAGNOSTIC_POP
    }

    template <usize ByteSize, same_as<const char> ConstChar, usize N>
        requires(N == ByteSize + 1)
    [[nodiscard]] constexpr bool is_equal(const not_null<const char*> a, ConstChar (&b)[N]) noexcept
    {
        SNN_DIAGNOSTIC_PUSH
        SNN_DIAGNOSTIC_IGNORE_UNSAFE_BUFFER_USAGE_IN_LIBC_CALL

        return __builtin_memcmp(a.get(), b, ByteSize) == 0;

        SNN_DIAGNOSTIC_POP
    }
}
