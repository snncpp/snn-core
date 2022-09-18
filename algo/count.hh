// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

// # Count all elements or elements equal to value

#pragma once

#include "snn-core/core.hh"

namespace snn::algo
{
    // ## Functions

    // ### count

    template <input_range Rng>
    [[nodiscard]] constexpr usize count(Rng rng)
    {
        if constexpr (random_access_range<Rng>)
        {
            return rng.count();
        }
        else
        {
            usize count = 0;
            while (rng)
            {
                rng.drop_front(promise::not_empty);
                ++count;
            }
            return count;
        }
    }

    template <input_range Rng, typename T>
    [[nodiscard]] constexpr usize count(Rng rng, const T& value)
    {
        usize count = 0;
        while (rng)
        {
            const auto& e = rng.front(promise::not_empty);
            if (e == value)
            {
                ++count;
            }
            rng.drop_front(promise::not_empty);
        }
        return count;
    }
}
