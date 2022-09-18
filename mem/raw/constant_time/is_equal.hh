// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

// # Is equal

#pragma once

#include "snn-core/core.hh"

namespace snn::mem::raw::constant_time
{
    // ## Functions

    // ### is_equal

    [[nodiscard]] __attribute__((noinline)) inline bool is_equal(const not_null<const void*> a,
                                                                 const not_null<const void*> b,
                                                                 const byte_size size) noexcept
    {
        // Volatile to prevent optimization, tested with:
        // * clang version 13.0.1
        // * g++12 (FreeBSD Ports Collection) 12.0.1 20220306 (experimental)

        volatile usize sz       = size.get();
        volatile const byte* aa = reinterpret_cast<const byte*>(a.get());
        volatile const byte* bb = reinterpret_cast<const byte*>(b.get());

        byte diff = 0;
        for (usize i = 0; i < sz; ++i)
        {
            // XOR bits in aa and bb at position i, OR result with diff and store in diff.
            diff |= static_cast<byte>(aa[i] ^ bb[i]);
        }
        // Will have no bits set if all bytes are equal.
        return diff == 0;
    }
}
