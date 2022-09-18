// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

// # Relocate objects to an uninitialized address N positions to the left

// The memory ranges may overlap.

#pragma once

#include "snn-core/mem/relocate.hh"

namespace snn::mem
{
    // ## Functions

    // ### relocate_left

    template <typename T>
        requires(std::is_move_constructible_v<T>)
    constexpr void relocate_left(const not_null<T*> first_relocate,
                                 const not_null<T*> last_relocate,
                                 const not_zero<usize> count) noexcept
    {
        mem::relocate(first_relocate, last_relocate, not_null{first_relocate.get() - count.get()});
    }
}
