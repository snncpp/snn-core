// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

// # Does no element match predicate

#pragma once

#include "snn-core/core.hh"

namespace snn::algo
{
    // ## Functions

    // ### none

    template <input_range Rng, typename OneArgPred>
    [[nodiscard]] constexpr bool none(Rng rng, OneArgPred p)
    {
        while (rng)
        {
            const auto& e = rng.front(assume::not_empty);
            if (p(e))
            {
                return false;
            }
            rng.drop_front(assume::not_empty);
        }
        return true;
    }
}
