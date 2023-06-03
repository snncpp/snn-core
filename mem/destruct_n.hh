// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

// # Destruct N objects at a given address

#pragma once

#include "snn-core/core.hh"

namespace snn::mem
{
    // ## Functions

    // ### destruct_n

    SNN_DIAGNOSTIC_PUSH
    SNN_DIAGNOSTIC_IGNORE_UNSAFE_BUFFER_USAGE

    template <typename T>
    constexpr void destruct_n(T* const first, const usize count) noexcept
    {
        snn_should(first != nullptr || count == 0);

        T* cur        = first;
        T* const last = cur + count;
        while (cur != last)
        {
            cur->~T();
            ++cur;
        }
    }

    SNN_DIAGNOSTIC_POP
}
