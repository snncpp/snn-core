// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

// # Relocate objects backward to an uninitialized address

// The memory ranges may overlap, but only if `last_uninitialized > last_relocate`.

#pragma once

#include "snn-core/fn/common.hh"
#include "snn-core/mem/construct.hh"
#include "snn-core/mem/destruct.hh"
#include "snn-core/mem/raw/is_overlapping.hh"
#include "snn-core/mem/raw/move.hh"

namespace snn::mem
{
    // ## Functions

    // ### relocate_backward

    template <typename T>
        requires(std::is_move_constructible_v<T>)
    constexpr void relocate_backward(const not_null<T*> first_relocate,
                                     const not_null<T*> last_relocate,
                                     const not_null<T*> last_uninitialized) noexcept
    {
        T* first         = first_relocate.get();
        T* last          = last_relocate.get();
        const auto count = static_cast<usize>(last - first);
        T* last_uninit   = last_uninitialized.get();
        T* first_uninit  = last_uninit - count;

#if SNN_SHOULD_BOOL
        snn_should(first <= last);
        if (!std::is_constant_evaluated())
        {
            snn_should(fn::less_than{}(last, last_uninit) ||
                       !mem::raw::is_overlapping(first, last, first_uninit, last_uninit));
        }
#endif

        if (!std::is_constant_evaluated() && is_trivially_relocatable_v<T>)
        {
            mem::raw::move(not_null<const T*>{first}, not_null{first_uninit},
                           byte_size{count * sizeof(T)});
        }
        else
        {
            while (first != last)
            {
                --last;
                --last_uninit;
                mem::construct(not_null{last_uninit}, std::move(*last));
                mem::destruct(not_null{last});
            }
        }
    }
}
