// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

// # Find first element matching predicate

#pragma once

#include "snn-core/optional_index.hh"

namespace snn::algo
{
    // ## Functions

    // ### find_if

    template <input_range Rng, typename OneArgPred>
    [[nodiscard]] constexpr optional_index find_if(Rng rng, OneArgPred p)
    {
        usize index = 0;
        while (rng)
        {
            const auto& e = rng.front(assume::not_empty);
            if (p(e))
            {
                return optional_index{index, assume::within_bounds};
            }
            rng.drop_front(assume::not_empty);
            ++index;
        }
        return constant::npos;
    }
}
