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
            val_or_ref prev{rng.front(assume::not_empty)};
            rng.drop_front(assume::not_empty);

            while (rng)
            {
                val_or_ref cur{rng.front(assume::not_empty)};
                rng.drop_front(assume::not_empty);

                if (is_less(std::as_const(cur.get()), std::as_const(prev.get())))
                {
                    return false;
                }

                prev.assign_or_rebind(std::move(cur).get());
            }
        }

        return true;
    }
}
