// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

// # Find value via binary search (lower bound)

#pragma once

#include "snn-core/optional_index.hh"
#include "snn-core/fn/common.hh"
#include <algorithm> // lower_bound

namespace snn::algo
{
    // ## Functions

    // ### find_greater_than_or_equal_to

    template <random_access_range RandomAccessRng, typename T, typename TwoArgPred>
        requires legacy_iterable<RandomAccessRng>
    [[nodiscard]] constexpr optional_index find_greater_than_or_equal_to(
        RandomAccessRng rng, const T& value, TwoArgPred is_less, promise::is_sorted_t)
    {
        const auto first = rng.begin();
        const auto last  = rng.end();
        const auto lower = std::lower_bound(first, last, value, std::move(is_less));
        if (lower != last)
        {
            return to_usize(lower - first);
        }
        return constant::npos;
    }

    template <random_access_range RandomAccessRng, typename T>
        requires legacy_iterable<RandomAccessRng>
    [[nodiscard]] constexpr optional_index find_greater_than_or_equal_to(RandomAccessRng rng,
                                                                         const T& value,
                                                                         promise::is_sorted_t)
    {
        return find_greater_than_or_equal_to(std::move(rng), value, fn::less_than{},
                                             promise::is_sorted);
    }
}
