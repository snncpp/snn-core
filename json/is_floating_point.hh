// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

// # Is floating point

// Does string match an arbitrary precision decimal floating point with a fraction and/or exponent.

#pragma once

#include "snn-core/chr/common.hh"
#include "snn-core/fn/common.hh"
#include "snn-core/range/contiguous.hh"

namespace snn::json
{
    // ## Functions

    // ### is_floating_point

    [[nodiscard]] constexpr bool is_floating_point(const transient<cstrview> s) noexcept
    {
        auto rng = s.get().range();

        rng.drop_front('-');

        // Leading zero or one or more digits.
        if (rng.drop_front('0') || rng.pop_front_while(chr::is_digit))
        {
            bool have_fraction_or_exponent = false;

            // Fraction
            if (rng.drop_front('.'))
            {
                if (!rng.pop_front_while(chr::is_digit))
                {
                    return false;
                }

                have_fraction_or_exponent = true;
            }

            // Exponent
            if (rng.drop_front_if(fn::in_array{'e', 'E'}))
            {
                rng.drop_front_if(fn::in_array{'+', '-'});

                if (!rng.pop_front_while(chr::is_digit))
                {
                    return false;
                }

                have_fraction_or_exponent = true;
            }

            return have_fraction_or_exponent && rng.is_empty();
        }

        return false;
    }
}
