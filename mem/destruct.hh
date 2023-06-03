// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

// # Destruct object(s) at a given address

#pragma once

#include "snn-core/core.hh"

namespace snn::mem
{
    // ## Functions

    // ### destruct

    template <typename T>
    constexpr void destruct(const not_null<T*> at) noexcept
    {
        at.get()->~T();
    }

    SNN_DIAGNOSTIC_PUSH
    SNN_DIAGNOSTIC_IGNORE_UNSAFE_BUFFER_USAGE

    template <typename T>
    constexpr void destruct(T* const first, T* const last) noexcept
    {
        snn_should(first == last || (first != nullptr && last != nullptr && first < last));

        T* cur = first;
        while (cur != last)
        {
            cur->~T();
            ++cur;
        }
    }

    SNN_DIAGNOSTIC_POP
}
