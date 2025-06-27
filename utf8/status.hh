// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

// # Get information about a string

#pragma once

#include "snn-core/result.hh"
#include "snn-core/ascii/skip.hh"
#include "snn-core/chr/common.hh"
#include "snn-core/range/contiguous.hh"
#include "snn-core/utf8/core.hh"
#include "snn-core/utf8/error.hh"

namespace snn::utf8
{
    // ## Classes

    // ### info

    struct info final
    {
        usize count;
        bool is_ascii;
    };

    // ## Functions

    // ### status

    [[nodiscard]] constexpr result<info> status(const transient<cstrview> s) noexcept
    {
        auto rng      = s.get().range();
        usize count   = 0;
        bool is_ascii = true;
        while (rng)
        {
            const char c = rng.front(assume::not_empty);
            if (chr::is_ascii(c))
            {
                count += rng.count();
                rng = ascii::skip(rng);
                count -= rng.count();
            }
            else
            {
                is_ascii     = false;
                const auto p = rng.pop_front_codepoint();
                if (p.value != utf8::invalid)
                {
                    ++count;
                }
                else
                {
                    return error::invalid;
                }
            }
        }
        return info{count, is_ascii};
    }
}
