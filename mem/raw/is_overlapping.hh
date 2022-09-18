// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

// # Is overlapping

#pragma once

#include "snn-core/array_view.fwd.hh"
#include "snn-core/cmp/common.hh"
#include "snn-core/fn/common.hh"

namespace snn::mem::raw
{
    // ## Functions

    // ### is_overlapping

    template <typename T>
    [[nodiscard]] constexpr bool is_overlapping(const T* a_first, const T* a_last, //
                                                const T* b_first, const T* b_last) noexcept
    {
        // Branchless (two conditional moves).
        // This uses std::less<void> which is well defined when comparing pointers.
        return fn::less_than{}(cmp::max(a_first, b_first), cmp::min(a_last, b_last));
    }

    template <typename T, usize Ca, usize Cb>
    [[nodiscard]] constexpr bool is_overlapping(const array_view<T, Ca> a,
                                                const array_view<T, Cb> b) noexcept
    {
        return is_overlapping(a.begin(), a.end(), b.begin(), b.end());
    }
}
