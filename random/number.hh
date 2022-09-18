// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

// # Random number (integral)

#pragma once

#include "snn-core/math/common.hh"
#include "snn-core/random/detail/source.hh"
#include "snn-core/random/detail/uniform.hh"

namespace snn::random
{
    // ## Functions

    // ### number

    template <strict_integral Int>
    [[nodiscard]] Int number()
    {
        if constexpr (sizeof(Int) <= sizeof(u32))
        {
            const u32 i = random::detail::source::number();
            return static_cast<Int>(i);
        }
        else
        {
            Int n = 0;
            random::detail::source::fill(strview{not_null{reinterpret_cast<char*>(&n)}, sizeof(n)});
            return n;
        }
    }

    template <strict_integral Int>
    [[nodiscard]] Int number(const Int min_inclusive, const Int max_exclusive)
    {
        if (min_inclusive < max_exclusive)
        {
            using Uint = std::make_unsigned_t<Int>;

            const Uint abs_diff = math::subtract_with_overflow(static_cast<Uint>(max_exclusive),
                                                               static_cast<Uint>(min_inclusive));
            const Uint rnd =
                random::detail::uniform(abs_diff, [] { return random::number<Uint>(); });

            // Safe overflow/wrap around for negative minimum.
            const auto i =
                static_cast<Int>(math::add_with_overflow(rnd, static_cast<Uint>(min_inclusive)));
            snn_should(i >= min_inclusive && i < max_exclusive);
            return i;
        }
        return min_inclusive;
    }
}
