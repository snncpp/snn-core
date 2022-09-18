// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

// # Return the greatest element

#pragma once

#include "snn-core/optional.hh"
#include "snn-core/val_or_ref.hh"
#include "snn-core/fn/common.hh"

namespace snn::algo
{
    // ## Functions

    // ### max

    template <input_range Rng, typename TwoArgPred = fn::less_than>
    [[nodiscard]] constexpr auto max(Rng rng, TwoArgPred is_less = TwoArgPred{})
        -> optional<result_compat_t<decltype(rng.front(promise::not_empty))>>
    {
        if (rng)
        {
            val_or_ref max{rng.front(promise::not_empty)};
            rng.drop_front(promise::not_empty);

            while (rng)
            {
                decltype(auto) e = rng.front(promise::not_empty);
                if (is_less(std::as_const(max.get()), std::as_const(e)))
                {
                    max.assign_or_rebind(std::forward<decltype(e)>(e));
                }
                rng.drop_front(promise::not_empty);
            }

            return std::move(max).get();
        }
        return nullopt;
    }
}
