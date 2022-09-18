// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

// # Is integral

// Does string match an arbitrary precision decimal integral.

#pragma once

#include "snn-core/array_view.hh"
#include "snn-core/chr/common.hh"
#include "snn-core/range/contiguous.hh"

namespace snn::json
{
    // ## Functions

    // ### is_unsigned_integral

    [[nodiscard]] constexpr bool is_unsigned_integral(const transient<cstrview> s) noexcept
    {
        auto rng = s.get().range();
        if (rng.drop_front('0'))
        {
            // Leading zero.
            return rng.is_empty();
        }
        return rng && rng.all(chr::is_digit);
    }

    // ### is_signed_integral

    [[nodiscard]] constexpr bool is_signed_integral(const transient<cstrview> s) noexcept
    {
        auto rng = s.get().range();
        return rng.drop_front('-') && json::is_unsigned_integral(cstrview{rng});
    }

    // ### is_integral

    [[nodiscard]] constexpr bool is_integral(const transient<cstrview> s) noexcept
    {
        auto rng = s.get().range();
        rng.drop_front('-');
        return json::is_unsigned_integral(cstrview{rng});
    }
}
