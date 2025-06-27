// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

// # Replace elements matching predicate

#pragma once

#include "snn-core/core.hh"

namespace snn::algo
{
    // ## Functions

    // ### replace_if

    template <input_range Rng, typename OneArgPred, typename V>
    constexpr void replace_if(Rng rng, OneArgPred p, const V& replacement)
    {
        while (rng)
        {
            auto& e = rng.front(assume::not_empty);
            if (p(std::as_const(e)))
            {
                e = replacement;
            }
            rng.drop_front(assume::not_empty);
        }
    }
}
