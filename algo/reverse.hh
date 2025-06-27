// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

// # Reverse range

#pragma once

#include "snn-core/range/unchecked/bidirectional.hh"

namespace snn::algo
{
    // ## Functions

    // ### reverse

    template <bidirectional_range BidirectionalRng>
    constexpr void reverse(BidirectionalRng rng)
    {
        if constexpr (random_access_range<BidirectionalRng> && legacy_iterable<BidirectionalRng>)
        {
            range::unchecked::bidirectional urng{init::from, rng.begin(), rng.end()};
            // Do `count / 2` (truncated) swaps.
            // This loop can typically be unrolled by the optimizer.
            for (loop::count lc{rng.count() / 2}; lc--;)
            {
                using std::swap;
                swap(urng.front(assume::not_empty), urng.back(assume::not_empty));

                urng.drop_front(assume::not_empty);
                urng.drop_back(assume::not_empty);
            }
        }
        else
        {
            while (rng)
            {
                decltype(auto) front_elem = rng.front(assume::not_empty);
                rng.drop_front(assume::not_empty);

                if (rng)
                {
                    decltype(auto) back_elem = rng.back(assume::not_empty);
                    rng.drop_back(assume::not_empty);

                    static_assert(std::is_lvalue_reference_v<decltype(front_elem)>);
                    static_assert(std::is_lvalue_reference_v<decltype(back_elem)>);

                    using std::swap;
                    swap(front_elem, back_elem);
                }
            }
        }
    }
}
