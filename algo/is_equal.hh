// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

// # Are two ranges equal

#pragma once

#include "snn-core/fn/common.hh"
#include "snn-core/range/contiguous.hh"
#include "snn-core/range/unchecked/forward.hh"

namespace snn::algo
{
    // ## Functions

    // ### is_equal

    // Compare two ranges for equality.

    template <input_range FirstRng, input_range SecondRng, typename TwoArgPred = fn::equal_to>
    [[nodiscard]] constexpr bool is_equal(FirstRng a, SecondRng b,
                                          TwoArgPred is_equal = TwoArgPred{})
    {
        if constexpr (random_access_range<FirstRng> && random_access_range<SecondRng> &&
                      legacy_iterable<SecondRng>)
        {
            if (a.count() != b.count())
            {
                return false;
            }

            range::unchecked::forward ub{init::from, b.begin(), b.end()};
            while (a)
            {
                if (!is_equal(a.front(promise::not_empty), ub.front(promise::not_empty)))
                {
                    return false;
                }
                a.drop_front(promise::not_empty);
                ub.drop_front(promise::not_empty);
            }

            return true;
        }
        else
        {
            while (a && b)
            {
                if (!is_equal(a.front(promise::not_empty), b.front(promise::not_empty)))
                {
                    return false;
                }
                a.drop_front(promise::not_empty);
                b.drop_front(promise::not_empty);
            }
            return a.is_empty() && b.is_empty();
        }
    }

    // Compare a range and an initializer_list for equality.

    template <input_range Rng, typename T, typename TwoArgPred = fn::equal_to>
    [[nodiscard]] constexpr bool is_equal(Rng a, initializer_list<T> b,
                                          TwoArgPred is_equal = TwoArgPred{})
    {
        return algo::is_equal(std::move(a), range::contiguous{init::from, b.begin(), b.end()},
                              std::move(is_equal));
    }
}
