// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

// # Unpack front element into variables

#pragma once

#include "snn-core/optional.hh"

namespace snn::algo
{
    // ## Functions

    // ### unpack_front

    template <input_range Rng>
    [[nodiscard]] constexpr optional<void> unpack_front(Rng)
    {
        return {};
    }

    template <input_range Rng, typename T, typename... Args>
    [[nodiscard]] constexpr optional<void> unpack_front(Rng rng, T& t, Args&... args)
    {
        // Do not overcomplicate this, the optimizer is pretty good at removing the redundant
        // checks for contiguous ranges.

        if (rng)
        {
            t = rng.front(assume::not_empty);
            rng.drop_front(assume::not_empty);

            return unpack_front(std::move(rng), args...);
        }

        return nullopt;
    }
}
