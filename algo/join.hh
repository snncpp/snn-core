// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

// # Join with or without delimiter

#pragma once

#include "snn-core/core.hh"

namespace snn::algo
{
    // ## Functions

    // ### join

    // Join without delimiter.

    template <input_range Rng, typename Container>
    constexpr void join(Rng rng, Container& append_to, assume::no_overlap_t)
    {
        // Note: There is no easy way to check if rng overlap append_to.
        while (rng)
        {
            append_to << rng.front(assume::not_empty);
            rng.drop_front(assume::not_empty);
        }
    }

    template <typename Container, input_range Rng>
    [[nodiscard]] constexpr Container join(Rng rng)
    {
        Container append_to;
        join(std::move(rng), append_to, assume::no_overlap);
        return append_to;
    }

    // Join with delimiter.

    template <input_range Rng, typename T, typename Container>
    constexpr void join(Rng rng, const T& delimiter, Container& append_to, assume::no_overlap_t)
    {
        // Note: There is no easy way to check if rng or delimiter overlap append_to.
        if (rng)
        {
            append_to << rng.front(assume::not_empty);
            rng.drop_front(assume::not_empty);

            while (rng)
            {
                append_to << delimiter << rng.front(assume::not_empty);
                rng.drop_front(assume::not_empty);
            }
        }
    }

    template <typename Container, input_range Rng, typename T>
    [[nodiscard]] constexpr Container join(Rng rng, const T& delimiter)
    {
        Container append_to;
        join(std::move(rng), delimiter, append_to, assume::no_overlap);
        return append_to;
    }
}
