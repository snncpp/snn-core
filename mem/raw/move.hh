// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

// # Move (memmove)

// A wrapper around `memmove`:
//  * Byte size is explicit.
//  * Will not implicitly cast non-const pointer to const pointer.

#pragma once

#include "snn-core/core.hh"

namespace snn::mem::raw
{
    // ## Functions

    // ### move

    template <typename From, typename To>
    constexpr void move(const not_null<const From*> from, const not_null<To*> to,
                        const byte_size<usize> size) noexcept
    {
        static_assert(is_trivially_relocatable_v<From>);
        static_assert(is_trivially_relocatable_v<To>);

        SNN_DIAGNOSTIC_PUSH
        SNN_DIAGNOSTIC_IGNORE_UNSAFE_BUFFER_USAGE_IN_LIBC_CALL
        SNN_DIAGNOSTIC_IGNORE_NONTRIVIAL_MEMCALL

        __builtin_memmove(to.get(), from.get(), size.get());

        SNN_DIAGNOSTIC_POP
    }

    template <usize ByteSize, typename From, typename To>
        requires(sizeof(From) == 1 && sizeof(To) == 1)
    constexpr void move(const not_null<const From*> from, const not_null<To*> to) noexcept
    {
        static_assert(is_trivially_relocatable_v<From>);
        static_assert(is_trivially_relocatable_v<To>);

        SNN_DIAGNOSTIC_PUSH
        SNN_DIAGNOSTIC_IGNORE_UNSAFE_BUFFER_USAGE_IN_LIBC_CALL
        SNN_DIAGNOSTIC_IGNORE_NONTRIVIAL_MEMCALL

        __builtin_memmove(to.get(), from.get(), ByteSize);

        SNN_DIAGNOSTIC_POP
    }
}
