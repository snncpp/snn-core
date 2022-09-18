// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

// # Is a range sorted

#pragma once

#include "snn-core/val_or_ref.hh"
#include "snn-core/fn/common.hh"

namespace snn::algo
{
    // ## Functions

    // ### is_sorted

    template <input_range Rng, typename TwoArgPred = fn::less_than>
    [[nodiscard]] constexpr bool is_sorted(Rng rng, TwoArgPred is_less = TwoArgPred{})
    {
        if (rng)
        {
            val_or_ref prev{rng.front(promise::not_empty)};
            rng.drop_front(promise::not_empty);

            while (rng)
            {
                decltype(auto) e = rng.front(promise::not_empty);
                if (is_less(std::as_const(e), std::as_const(prev.get())))
                {
                    return false;
                }
                prev.assign_or_rebind(std::forward<decltype(e)>(e));
                rng.drop_front(promise::not_empty);
            }
        }
        return true;
    }
}
