// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

// # Transform all elements with a function object

#pragma once

#include "snn-core/core.hh"

namespace snn::algo
{
    // ## Functions

    // ### transform

    template <input_range Rng, typename OneArgOp>
    constexpr void transform(Rng rng, OneArgOp op)
    {
        while (rng)
        {
            auto& e = rng.front(assume::not_empty);
            e       = op(std::as_const(e));
            rng.drop_front(assume::not_empty);
        }
    }
}
