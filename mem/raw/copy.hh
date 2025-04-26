// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

// # Copy (memcpy)

// A wrapper around `memcpy`:
//  * Byte size is explicit.
//  * Will not implicitly cast non-const pointer to const pointer.
//  * A `no_overlap` promise is required.

#pragma once

#include "snn-core/mem/raw/is_overlapping.hh"

namespace snn::mem::raw
{
    // ## Functions

    // ### copy

    template <typename From, typename To>
    constexpr void copy(const not_null<const From*> from, const not_null<To*> to,
                        const byte_size<usize> size, promise::no_overlap_t) noexcept
    {
        static_assert(std::is_trivially_copyable_v<From>);
        static_assert(std::is_trivially_copyable_v<To>);

#if SNN_SHOULD_ENABLED
        if (!std::is_constant_evaluated())
        {
            SNN_DIAGNOSTIC_PUSH
            SNN_DIAGNOSTIC_IGNORE_UNSAFE_BUFFER_USAGE

            const byte* const from_first = reinterpret_cast<const byte*>(from.get());
            const byte* const from_last  = from_first + size.get();
            const byte* const to_first   = reinterpret_cast<const byte*>(to.get());
            const byte* const to_last    = to_first + size.get();
            snn_should(!mem::raw::is_overlapping(from_first, from_last, to_first, to_last));

            SNN_DIAGNOSTIC_POP
        }
#endif

        SNN_DIAGNOSTIC_PUSH
        SNN_DIAGNOSTIC_IGNORE_UNSAFE_BUFFER_USAGE_IN_LIBC_CALL

        __builtin_memcpy(to.get(), from.get(), size.get());

        SNN_DIAGNOSTIC_POP
    }

    template <usize ByteSize, typename From, typename To>
        requires(sizeof(From) == 1 && sizeof(To) == 1)
    constexpr void copy(const not_null<const From*> from, const not_null<To*> to,
                        promise::no_overlap_t) noexcept
    {
        static_assert(std::is_trivially_copyable_v<From>);
        static_assert(std::is_trivially_copyable_v<To>);

#if SNN_SHOULD_ENABLED
        if (!std::is_constant_evaluated())
        {
            SNN_DIAGNOSTIC_PUSH
            SNN_DIAGNOSTIC_IGNORE_UNSAFE_BUFFER_USAGE

            const byte* const from_first = reinterpret_cast<const byte*>(from.get());
            const byte* const from_last  = from_first + ByteSize;
            const byte* const to_first   = reinterpret_cast<const byte*>(to.get());
            const byte* const to_last    = to_first + ByteSize;
            snn_should(!mem::raw::is_overlapping(from_first, from_last, to_first, to_last));

            SNN_DIAGNOSTIC_POP
        }
#endif

        SNN_DIAGNOSTIC_PUSH
        SNN_DIAGNOSTIC_IGNORE_UNSAFE_BUFFER_USAGE_IN_LIBC_CALL

        __builtin_memcpy(to.get(), from.get(), ByteSize);

        SNN_DIAGNOSTIC_POP
    }
}
