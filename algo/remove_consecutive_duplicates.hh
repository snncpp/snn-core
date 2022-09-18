// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

// # Remove consecutive element duplicates

#pragma once

#include "snn-core/fn/common.hh"
#include <algorithm> // unique

namespace snn::algo
{
    // ## Functions

    // ### remove_consecutive_duplicates

    template <forward_range ForwardRng, typename TwoArgPred = fn::equal_to>
        requires legacy_iterable<ForwardRng> && constructible_from_iterators<ForwardRng>
    [[nodiscard]] constexpr ForwardRng remove_consecutive_duplicates(
        ForwardRng rng, TwoArgPred is_equal = TwoArgPred{})
    {
        const auto first      = rng.begin();
        const auto last       = rng.end();
        const auto last_after = std::unique(first, last, std::move(is_equal));
        return ForwardRng{meta::iterators, first, last_after};
    }
}
