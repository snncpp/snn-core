// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

// # Is equal ignoring case

#pragma once

#include "snn-core/array_view.hh"
#include "snn-core/ascii/fn/equal_to_icase.hh"

namespace snn::ascii
{
    // ## Functions

    // ### is_equal_icase

    [[nodiscard]] constexpr bool is_equal_icase(const transient<cstrview> a,
                                                const transient<cstrview> b) noexcept
    {
        return ascii::fn::equal_to_icase{}(a, b);
    }
}
