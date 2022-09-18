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
                        const byte_size size) noexcept
    {
        __builtin_memmove(to.get(), from.get(), size.get());
    }

    template <usize ByteSize, typename From, typename To>
        requires(sizeof(From) == 1 && sizeof(To) == 1)
    constexpr void move(const not_null<const From*> from, const not_null<To*> to) noexcept
    {
        __builtin_memmove(to.get(), from.get(), ByteSize);
    }
}
