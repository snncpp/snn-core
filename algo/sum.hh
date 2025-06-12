// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

// # Get a sum of all elements

#pragma once

#include "snn-core/algo/reduce.hh"
#include "snn-core/fn/common.hh"
#include "snn-core/num/safe.hh"

namespace snn::algo
{
    // ## Functions

    // ### sum

    template <input_range Rng, typename T>
    [[nodiscard]] constexpr T sum(Rng rng, T initial_value)
    {
        return algo::reduce(std::move(rng), std::move(initial_value), fn::add{});
    }

    template <input_range Rng>
    [[nodiscard]] constexpr auto sum(Rng rng)
    {
        return sum(std::move(rng), num::safe<front_value_t<Rng&>>{});
    }
}
