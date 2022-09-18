// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

// # Relocate objects to an uninitialized address

// The memory ranges may overlap, but only if `first_uninitialized < first_relocate`.

#pragma once

#include "snn-core/fn/common.hh"
#include "snn-core/mem/construct.hh"
#include "snn-core/mem/destruct.hh"
#include "snn-core/mem/raw/is_overlapping.hh"
#include "snn-core/mem/raw/move.hh"

namespace snn::mem
{
    // ## Functions

    // ### relocate

    template <typename T>
        requires(std::is_move_constructible_v<T>)
    constexpr void relocate(const not_null<T*> first_relocate, const not_null<T*> last_relocate,
                            const not_null<T*> first_uninitialized) noexcept
    {
        T* first         = first_relocate.get();
        T* last          = last_relocate.get();
        const auto count = static_cast<usize>(last - first);
        T* first_uninit  = first_uninitialized.get();
        T* last_uninit   = first_uninit + count;

        ignore_if_unused(last_uninit);

#if SNN_SHOULD_BOOL
        snn_should(first <= last);
        if (!std::is_constant_evaluated())
        {
            snn_should(fn::less_than{}(first_uninit, first) ||
                       !mem::raw::is_overlapping(first, last, first_uninit, last_uninit));
        }
#endif

        if (!std::is_constant_evaluated() && is_trivially_relocatable_v<T>)
        {
            mem::raw::move(not_null<const T*>{first}, first_uninitialized,
                           byte_size{count * sizeof(T)});
        }
        else
        {
            while (first != last)
            {
                mem::construct(not_null{first_uninit}, std::move(*first));
                mem::destruct(not_null{first});
                ++first;
                ++first_uninit;
            }
        }
    }
}
