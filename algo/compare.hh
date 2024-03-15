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
        -> decltype(a.front(promise::not_empty) <=> b.front(promise::not_empty))
    {
        if constexpr (random_access_range<FirstRng> && legacy_iterable<FirstRng> &&
                      random_access_range<SecondRng> && legacy_iterable<SecondRng>)
        {
            range::unchecked::forward ua{meta::iterators, a.begin(), a.end()};
            range::unchecked::forward ub{meta::iterators, b.begin(), b.end()};
            for (loop::count lc{math::min(a.count(), b.count())}; lc--;)
            {
                const auto cmp = ua.front(promise::not_empty) <=> ub.front(promise::not_empty);
                if (cmp != 0)
                {
                    return cmp;
                }
                ua.drop_front(promise::not_empty);
                ub.drop_front(promise::not_empty);
            }
            return a.count() <=> b.count();
        }
        else
        {
            while (a && b)
            {
                const auto cmp = a.front(promise::not_empty) <=> b.front(promise::not_empty);
                if (cmp != 0)
                {
                    return cmp;
                }
                a.drop_front(promise::not_empty);
                b.drop_front(promise::not_empty);
            }
            return !a.is_empty() <=> !b.is_empty();
        }
    }
}
