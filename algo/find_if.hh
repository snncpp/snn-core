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
            const auto& e = rng.front(promise::not_empty);
            if (p(e))
            {
                return index;
            }
            rng.drop_front(promise::not_empty);
            ++index;
        }
        return constant::npos;
    }
}
