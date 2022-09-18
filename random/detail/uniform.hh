// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

#pragma once

#include "snn-core/math/common.hh"

namespace snn::random::detail
{
    // Return an integer between 0 and upper bound (exclusive) without "modulo bias",
    // that is each number in the range has equal probably.

    template <unsigned_integral Uint, typename Generator>
    Uint uniform(const Uint upper_bound, Generator generator)
    {
        static_assert(std::is_same_v<decltype(generator()), Uint>);

        if (upper_bound <= 1)
        {
            return 0;
        }

        // Pick a minimum so that `rnd % upper_bound` will not be biased.
        const Uint rnd_min = math::negate_with_overflow(upper_bound) % upper_bound;
        snn_should(rnd_min <= (constant::limit<Uint>::max / 2)); // Worst case.

        while (true)
        {
            const Uint rnd = generator();
            if (rnd >= rnd_min)
            {
                return rnd % upper_bound;
            }
        }
    }
}
