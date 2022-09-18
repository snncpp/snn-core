// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

// # Fill range with value

#pragma once

#include "snn-core/core.hh"

namespace snn::algo
{
    // ## Functions

    // ### fill

    template <input_range Rng, typename V>
    constexpr void fill(Rng rng, const V& value)
    {
        while (rng)
        {
            auto& e = rng.front(promise::not_empty);
            e       = value;
            rng.drop_front(promise::not_empty);
        }
    }
}
