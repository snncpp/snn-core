// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

// # Count code points

// Fails on invalid UTF-8 input.

#pragma once

#include "snn-core/result.hh"
#include "snn-core/ascii/skip.hh"
#include "snn-core/chr/common.hh"
#include "snn-core/range/contiguous.hh"
#include "snn-core/utf8/error.hh"
#include "snn-core/utf8/core.hh"

namespace snn::utf8
{
    // ## Functions

    // ### count

    [[nodiscard]] constexpr result<usize> count(const transient<cstrview> s) noexcept
    {
        auto rng       = s.get().range();
        usize cp_count = 0;
        while (rng)
        {
            const char c = rng.front(promise::not_empty);
            if (chr::is_ascii(c))
            {
                cp_count += rng.count();
                rng = ascii::skip(rng);
                cp_count -= rng.count();
            }
            else
            {
                const auto p = rng.pop_front_codepoint();
                if (p.value != utf8::invalid)
                {
                    ++cp_count;
                }
                else
                {
                    return error::invalid;
                }
            }
        }
        return cp_count;
    }
}
