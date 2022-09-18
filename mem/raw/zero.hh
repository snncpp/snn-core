// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

// # Fill with zeros

// This operation will not be optimized away by the compiler and can be used to clear sensitive
// data in memory.

#pragma once

#include "snn-core/core.hh"
#if defined(__linux__)
#include <string.h> // explicit_bzero
#else
#include <strings.h> // explicit_bzero
#endif

namespace snn::mem::raw
{
    // ## Functions

    // ### zero

    template <typename T>
    void zero(const not_null<T*> destination, const byte_size size) noexcept
    {
        ::explicit_bzero(destination.get(), size.get());
    }

    template <typename T>
    void zero(T* const destination, const byte_size size) noexcept
    {
        snn_assert(size.get() == 0 || destination != nullptr);
        if (destination != nullptr)
        {
            zero(not_null{destination}, size);
        }
    }

    template <typename T>
    void zero(T& container) noexcept
    {
        zero(container.writable(), container.byte_size());
    }
}
