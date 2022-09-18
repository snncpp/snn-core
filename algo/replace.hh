// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

// # Replace elements equal to value

#pragma once

#include "snn-core/core.hh"

namespace snn::algo
{
    // ## Functions

    // ### replace

    template <input_range Rng, typename T, typename U>
    constexpr void replace(Rng rng, const T& needle, const U& replacement)
    {
        while (rng)
        {
            auto& e = rng.front(promise::not_empty);
            if (e == needle)
            {
                e = replacement;
            }
            rng.drop_front(promise::not_empty);
        }
    }
}
