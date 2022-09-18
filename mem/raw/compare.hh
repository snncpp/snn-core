// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

// # Compare (memcmp)

#pragma once

#include "snn-core/core.hh"

namespace snn::mem::raw
{
    // ## Functions

    // ### compare

    template <typename A, typename B>
    [[nodiscard]] constexpr int compare(const not_null<A*> a, const not_null<B*> b,
                                        const byte_size size) noexcept
    {
        return __builtin_memcmp(a.get(), b.get(), size.get());
    }

    template <usize ByteSize, typename A, typename B>
        requires(sizeof(A) == 1 && sizeof(B) == 1)
    [[nodiscard]] constexpr int compare(const not_null<A*> a, const not_null<B*> b) noexcept
    {
        return __builtin_memcmp(a.get(), b.get(), ByteSize);
    }
}
