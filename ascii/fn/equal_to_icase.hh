// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

// # Equal-to comparison ignoring case

#pragma once

#include "snn-core/array_view.hh"
#include "snn-core/algo/is_equal.hh"
#include "snn-core/chr/common.hh"
#include "snn-core/range/contiguous.hh"

namespace snn::ascii::fn
{
    // ## Classes

    // ### equal_to_icase

    struct equal_to_icase final
    {
        explicit equal_to_icase() = default;

        [[nodiscard]] constexpr bool operator()(const char a, const char b) const noexcept
        {
            return chr::to_alpha_lower(a) == chr::to_alpha_lower(b);
        }

        [[nodiscard]] constexpr bool operator()(const transient<cstrview> a,
                                                const transient<cstrview> b) const noexcept
        {
            return algo::is_equal(a.get().range(), b.get().range(), equal_to_icase{});
        }
    };

    // ## Aliases

    // ### eq_icase

    using eq_icase = equal_to_icase;
}
