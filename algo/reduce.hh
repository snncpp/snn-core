// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

// # Reduce elements down to a single value

#pragma once

#include "snn-core/core.hh"

namespace snn::algo
{
    // ## Functions

    // ### reduce

    template <input_range Rng, typename T, typename TwoArgOp>
    [[nodiscard]] constexpr T reduce(Rng rng, T init, TwoArgOp op)
    {
        while (rng)
        {
            const auto& e = rng.front(promise::not_empty);
            init          = op(std::move(init), e);
            rng.drop_front(promise::not_empty);
        }
        return init;
    }
}
