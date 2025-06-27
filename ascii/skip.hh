// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

// # Skip characters in front of range

#pragma once

#include "snn-core/array_view.hh"
#include "snn-core/chr/common.hh"
#include "snn-core/range/contiguous.hh"

namespace snn::ascii
{
    // ## Functions

    // ### skip

    template <character Char>
    [[nodiscard]] constexpr range::contiguous<Char*> skip(range::contiguous<Char*> rng) noexcept
    {
        static_assert(sizeof(u64) == 8);
        while (rng.size() >= sizeof(u64))
        {
            const array_view<const char, sizeof(u64)> v{rng.begin(), assume::has_capacity};
            const auto i = v.load<u64>();
            if ((i & 0x8080808080808080u) == 0) // 0x80 == 0b1000'0000
            {
                rng.pop_front_n(sizeof(u64));
            }
            else
            {
                break;
            }
        }
        rng.pop_front_while(chr::is_ascii);
        return rng;
    }
}
