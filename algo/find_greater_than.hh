// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

// # Find value via binary search (upper bound)

#pragma once

#include "snn-core/optional_index.hh"
#include "snn-core/fn/common.hh"
#include <algorithm> // upper_bound

namespace snn::algo
{
    // ## Functions

    // ### find_greater_than

    template <random_access_range RandomAccessRng, typename T, typename TwoArgPred>
        requires legacy_iterable<RandomAccessRng>
    [[nodiscard]] constexpr optional_index find_greater_than(RandomAccessRng rng, const T& value,
                                                             TwoArgPred is_less,
                                                             promise::is_sorted_t)
    {
        const auto first = rng.begin();
        const auto last  = rng.end();
        const auto upper = std::upper_bound(first, last, value, std::move(is_less));
        if (upper != last)
        {
            return to_usize(upper - first);
        }
        return constant::npos;
    }

    template <random_access_range RandomAccessRng, typename T>
        requires legacy_iterable<RandomAccessRng>
    [[nodiscard]] constexpr optional_index find_greater_than(RandomAccessRng rng, const T& value,
                                                             promise::is_sorted_t)
    {
        return find_greater_than(std::move(rng), value, fn::less_than{}, promise::is_sorted);
    }
}
