// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

// # Call function object with each element

#pragma once

#include "snn-core/core.hh"

namespace snn::algo
{
    // ## Functions

    // ### each

    template <input_range Rng, typename OneArgFn>
    constexpr void each(Rng rng, OneArgFn f)
    {
        while (rng)
        {
            f(rng.front(assume::not_empty));
            rng.drop_front(assume::not_empty);
        }
    }
}
