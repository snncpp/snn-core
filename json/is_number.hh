// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

// # Is number

// Does string match an arbitrary precision decimal number with an optional fraction/exponent.

#pragma once

#include "snn-core/chr/common.hh"
#include "snn-core/fn/common.hh"
#include "snn-core/range/contiguous.hh"

namespace snn::json
{
    // ## Functions

    // ### is_number

    [[nodiscard]] constexpr bool is_number(const transient<cstrview> s) noexcept
    {
        auto rng = s.get().range();

        // Optional minus sign.
        rng.drop_front('-');

        // Leading zero or one or more digits.
        if (rng.drop_front('0') || rng.pop_front_while(chr::is_digit))
        {
            // Fraction
            if (rng.drop_front('.'))
            {
                if (!rng.pop_front_while(chr::is_digit))
                {
                    return false;
                }
            }

            // Exponent
            if (rng.drop_front_if(fn::in_array{'e', 'E'}))
            {
                // Optional plus/minus sign.
                rng.drop_front_if(fn::in_array{'+', '-'});

                if (!rng.pop_front_while(chr::is_digit))
                {
                    return false;
                }
            }

            return rng.is_empty();
        }

        return false;
    }
}
