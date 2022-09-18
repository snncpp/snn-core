// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

// # Count elements matching predicate

#pragma once

#include "snn-core/core.hh"

namespace snn::algo
{
    // ## Functions

    // ### count_if

    template <input_range Rng, typename OneArgPred>
    [[nodiscard]] constexpr usize count_if(Rng rng, OneArgPred p)
    {
        usize count = 0;
        while (rng)
        {
            const auto& e = rng.front(promise::not_empty);
            if (p(e))
            {
                ++count;
            }
            rng.drop_front(promise::not_empty);
        }
        return count;
    }
}
