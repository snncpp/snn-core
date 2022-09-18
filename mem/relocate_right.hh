// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

// # Relocate objects to an uninitialized address N positions to the right

// The memory ranges may overlap.

#pragma once

#include "snn-core/mem/relocate_backward.hh"

namespace snn::mem
{
    // ## Functions

    // ### relocate_right

    template <typename T>
        requires(std::is_move_constructible_v<T>)
    constexpr void relocate_right(const not_null<T*> first_relocate,
                                  const not_null<T*> last_relocate,
                                  const not_zero<usize> count) noexcept
    {
        mem::relocate_backward(first_relocate, last_relocate,
                               not_null{last_relocate.get() + count.get()});
    }
}
