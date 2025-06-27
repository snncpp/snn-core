// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

// # Compare two ranges

#pragma once

#include "snn-core/math/common.hh"
#include "snn-core/range/unchecked/forward.hh"

namespace snn::algo
{
    // ## Functions

    // ### compare

    template <input_range FirstRng, input_range SecondRng>
    [[nodiscard]] constexpr auto compare(FirstRng a, SecondRng b) //
        -> decltype(a.front(assume::not_empty) <=> b.front(assume::not_empty))
    {
        if constexpr (random_access_range<FirstRng> && legacy_iterable<FirstRng> &&
                      random_access_range<SecondRng> && legacy_iterable<SecondRng>)
        {
            range::unchecked::forward ua{init::from, a.begin(), a.end()};
            range::unchecked::forward ub{init::from, b.begin(), b.end()};
            for (loop::count lc{math::min(a.count(), b.count())}; lc--;)
            {
                const auto cmp = ua.front(assume::not_empty) <=> ub.front(assume::not_empty);
                if (cmp != 0)
                {
                    return cmp;
                }
                ua.drop_front(assume::not_empty);
                ub.drop_front(assume::not_empty);
            }
            return a.count() <=> b.count();
        }
        else
        {
            while (a && b)
            {
                const auto cmp = a.front(assume::not_empty) <=> b.front(assume::not_empty);
                if (cmp != 0)
                {
                    return cmp;
                }
                a.drop_front(assume::not_empty);
                b.drop_front(assume::not_empty);
            }
            return !a.is_empty() <=> !b.is_empty();
        }
    }
}
