// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

// # Move objects to an uninitialized address

#pragma once

#include "snn-core/mem/construct.hh"

namespace snn::mem
{
    // ## Functions

    // ### move_construct

    template <typename T>
        requires(std::is_move_constructible_v<T>)
    constexpr void move_construct(const not_null<T*> first_move, const not_null<T*> last_move,
                                  const not_null<T*> first_uninitialized,
                                  promise::no_overlap_t) noexcept
    {
        T* first        = first_move.get();
        T* last         = last_move.get();
        T* first_uninit = first_uninitialized.get();

        snn_should(first <= last);
        while (first != last)
        {
            mem::construct(not_null{first_uninit}, std::move(*first));
            ++first;
            ++first_uninit;
        }
    }
}
