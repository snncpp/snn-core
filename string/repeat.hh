// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

// # Repeat string N times

#pragma once

#include "snn-core/strcore.hh"
#include "snn-core/num/safe.hh"

namespace snn::string
{
    // ## Functions

    // ### repeat

    template <typename Buf>
    constexpr void repeat(const transient<cstrview> string, const usize times,
                          strcore<Buf>& append_to, promise::no_overlap_t)
    {
        const cstrview s = string.get();
        snn_should(std::is_constant_evaluated() || !s.overlaps(append_to));
        if (s)
        {
            const usize fill_size = num::safe{s.size()}.multiply(times).value();
            append_to.append_for_overwrite(fill_size).fill(s);
        }
    }

    template <any_strcore Str = str>
    [[nodiscard]] constexpr Str repeat(const transient<cstrview> string, const usize times)
    {
        Str append_to;
        repeat(string, times, append_to, promise::no_overlap);
        return append_to;
    }
}
