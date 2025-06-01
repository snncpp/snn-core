// Copyright (c) 2025 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

// # Sort range (forward declare)

#pragma once

#include "snn-core/fn/common.hh"

namespace snn::algo
{
    // ## Functions

    // ### sort

    template <random_access_range RandomAccessRng, typename TwoArgPred = fn::less_than>
        requires legacy_iterable<RandomAccessRng>
    constexpr void sort(RandomAccessRng rng, TwoArgPred is_less = TwoArgPred{});
}
