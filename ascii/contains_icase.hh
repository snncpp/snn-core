// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

// # Contains ignoring case

#pragma once

#include "snn-core/array_view.hh"
#include "snn-core/algo/contains.hh"
#include "snn-core/ascii/fn/equal_to_icase.hh"
#include "snn-core/range/contiguous.hh"

namespace snn::ascii
{
    // ## Functions

    // ### contains_icase

    [[nodiscard]] constexpr bool contains_icase(const transient<cstrview> subject,
                                                const transient<cstrview> needle) noexcept
    {
        return algo::contains(subject.get().range(), needle.get().range(),
                              ascii::fn::equal_to_icase{});
    }
}
