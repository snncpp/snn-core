// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

// # Does all elements match predicate

#pragma once

#include "snn-core/core.hh"

namespace snn::algo
{
    // ## Functions

    // ### all

    template <input_range Rng, typename OneArgPred>
    [[nodiscard]] constexpr bool all(Rng rng, OneArgPred p)
    {
        while (rng)
        {
            const auto& e = rng.front(promise::not_empty);
            if (!p(e))
            {
                return false;
            }
            rng.drop_front(promise::not_empty);
        }
        return true;
    }
}
