// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

// # Does any element match predicate

#pragma once

#include "snn-core/core.hh"

namespace snn::algo
{
    // ## Functions

    // ### any

    template <input_range Rng, typename OneArgPred>
    [[nodiscard]] constexpr bool any(Rng rng, OneArgPred p)
    {
        while (rng)
        {
            const auto& e = rng.front(assume::not_empty);
            if (p(e))
            {
                return true;
            }
            rng.drop_front(assume::not_empty);
        }
        return false;
    }
}
