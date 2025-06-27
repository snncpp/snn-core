// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

// # Skip valid bytes in front of range

#pragma once

#include "snn-core/ascii/skip.hh"
#include "snn-core/chr/common.hh"
#include "snn-core/range/contiguous.hh"
#include "snn-core/utf8/core.hh"

namespace snn::utf8
{
    // ## Functions

    // ### skip

    template <character Char>
    [[nodiscard]] constexpr snn::range::contiguous<Char*> skip(
        snn::range::contiguous<Char*> rng) noexcept
    {
        while (rng)
        {
            const char c = rng.front(assume::not_empty);
            if (chr::is_ascii(c))
            {
                rng = ascii::skip(rng);
            }
            else
            {
                const auto rng_before = rng;
                const auto p          = rng.pop_front_codepoint();
                if (p.value == utf8::invalid)
                {
                    return rng_before;
                }
            }
        }
        return rng;
    }
}
