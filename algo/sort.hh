// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

// # Sort range

#pragma once

#include "snn-core/fn/common.hh"
#include <algorithm> // sort

namespace snn::algo
{
    // ## Functions

    // ### sort

    template <random_access_range RandomAccessRng, typename TwoArgPred = fn::less_than>
        requires legacy_iterable<RandomAccessRng>
    constexpr void sort(RandomAccessRng rng, TwoArgPred is_less = TwoArgPred{})
    {
        using front_type = decltype(rng.front(promise::not_empty));
        static_assert(std::is_reference_v<front_type>, "Range must be non-generating.");
        static_assert(!std::is_const_v<std::remove_reference_t<front_type>>,
                      "Range must be non-const.");
        std::sort(rng.begin(), rng.end(), std::move(is_less));
    }
}
