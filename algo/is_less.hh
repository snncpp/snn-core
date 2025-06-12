// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

// # Is a range less than another range

#pragma once

#include "snn-core/fn/common.hh"
#include "snn-core/math/common.hh"
#include "snn-core/range/unchecked/forward.hh"

namespace snn::algo
{
    // ## Functions

    // ### is_less

    template <input_range FirstRng, input_range SecondRng, typename TwoArgPred = fn::less_than>
    [[nodiscard]] constexpr bool is_less(FirstRng a, SecondRng b, TwoArgPred is_less = TwoArgPred{})
    {
        if constexpr (random_access_range<FirstRng> && legacy_iterable<FirstRng> &&
                      random_access_range<SecondRng> && legacy_iterable<SecondRng>)
        {
            range::unchecked::forward ua{init::from, a.begin(), a.end()};
            range::unchecked::forward ub{init::from, b.begin(), b.end()};
            for (loop::count lc{math::min(a.count(), b.count())}; lc--;)
            {
                const auto& e1 = ua.front(promise::not_empty);
                const auto& e2 = ub.front(promise::not_empty);
                if (is_less(e1, e2))
                {
                    return true;
                }
                if (is_less(e2, e1))
                {
                    return false;
                }
                ua.drop_front(promise::not_empty);
                ub.drop_front(promise::not_empty);
            }
            return a.count() < b.count();
        }
        else
        {
            while (a && b)
            {
                const auto& e1 = a.front(promise::not_empty);
                const auto& e2 = b.front(promise::not_empty);
                if (is_less(e1, e2))
                {
                    return true;
                }
                if (is_less(e2, e1))
                {
                    return false;
                }
                a.drop_front(promise::not_empty);
                b.drop_front(promise::not_empty);
            }
            return a.is_empty() && !b.is_empty();
        }
    }
}
