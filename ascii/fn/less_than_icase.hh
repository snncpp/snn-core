// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

// # Less-than comparison ignoring case

#pragma once

#include "snn-core/array_view.hh"
#include "snn-core/algo/is_less.hh"
#include "snn-core/chr/common.hh"
#include "snn-core/range/contiguous.hh"

namespace snn::ascii::fn
{
    // ## Classes

    // ### less_than_icase

    struct less_than_icase final
    {
        explicit less_than_icase() = default;

        [[nodiscard]] constexpr bool operator()(const char a, const char b) const noexcept
        {
            return chr::to_alpha_lower(a) < chr::to_alpha_lower(b);
        }

        [[nodiscard]] constexpr bool operator()(const transient<cstrview> a,
                                                const transient<cstrview> b) const noexcept
        {
            return algo::is_less(a.get().range(), b.get().range(), less_than_icase{});
        }
    };

    // ## Aliases

    // ### lt_icase

    using lt_icase = less_than_icase;
}
