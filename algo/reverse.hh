// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

// # Reverse range

#pragma once

#include "snn-core/core.hh"
#include <algorithm> // reverse

namespace snn::algo
{
    // ## Functions

    // ### reverse

    template <bidirectional_range BidirectionalRng>
        requires legacy_iterable<BidirectionalRng>
    constexpr void reverse(BidirectionalRng rng)
    {
        std::reverse(rng.begin(), rng.end());
    }
}
