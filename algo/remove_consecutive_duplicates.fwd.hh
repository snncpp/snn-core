// Copyright (c) 2025 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

// # Remove consecutive element duplicates (forward declare)

#pragma once

#include "snn-core/fn/common.hh"

namespace snn::algo
{
    // ## Functions

    // ### remove_consecutive_duplicates

    template <forward_range ForwardRng, typename TwoArgPred = fn::equal_to>
        requires legacy_iterable<ForwardRng> && constructible_from_iterators<ForwardRng>
    [[nodiscard]] constexpr ForwardRng remove_consecutive_duplicates(
        ForwardRng rng, TwoArgPred is_equal = TwoArgPred{});
}
