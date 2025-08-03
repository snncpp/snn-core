// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

// # Find last element matching predicate

#pragma once

#include "snn-core/optional_index.hh"

namespace snn::algo
{
    // ## Functions

    // ### find_in_reverse_if

    template <random_access_range RandomAccessRng, typename OneArgPred>
    [[nodiscard]] constexpr optional_index find_in_reverse_if(RandomAccessRng rng, OneArgPred p)
    {
        usize index = rng.count();
        while (rng)
        {
            --index;
            const auto& e = rng.back(assume::not_empty);
            if (p(e))
            {
                return optional_index{index, assume::within_bounds};
            }
            rng.drop_back(assume::not_empty);
        }
        return constant::npos;
    }
}
