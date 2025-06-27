// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

// # Count code points

// Invalid UTF-8 is counted as bytes.

#pragma once

#include "snn-core/ascii/skip.hh"
#include "snn-core/chr/common.hh"
#include "snn-core/range/contiguous.hh"

namespace snn::utf8::unchecked
{
    // ## Functions

    // ### count

    [[nodiscard]] constexpr usize count(const transient<cstrview> s) noexcept
    {
        auto rng       = s.get().range();
        usize cp_count = 0;
        while (rng)
        {
            const char c = rng.front(assume::not_empty);
            if (chr::is_ascii(c))
            {
                cp_count += rng.count();
                rng = ascii::skip(rng);
                cp_count -= rng.count();
            }
            else
            {
                rng.pop_front_codepoint();
                ++cp_count;
            }
        }
        return cp_count;
    }
}
