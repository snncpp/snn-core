// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

// # Normalize line endings

// "\r\n" and '\r' are replaced with '\n'.

#pragma once

#include "snn-core/strcore.hh"
#include "snn-core/range/unchecked/contiguous.hh"

namespace snn::string
{
    // ## Functions

    // ### normalize_line_endings

    template <character Char>
    [[nodiscard]] constexpr range::contiguous<Char*> normalize_line_endings(
        range::contiguous<Char*> rng) noexcept
    {
        const usize pos = rng.find('\r').value_or_npos();
        if (pos != constant::npos)
        {
            const auto initial_rng = rng;

            rng.pop_front_n(pos);

            snn::range::unchecked::contiguous write_rng{init::from, rng.begin(), rng.end()};
            snn_should(write_rng && write_rng.front(assume::not_empty) == '\r');

            while (rng)
            {
                char c = rng.pop_front(assume::not_empty);
                if (c == '\r')
                {
                    c = '\n';
                    rng.drop_front('\n'); // Skip '\n' in "\r\n" if any.
                }

                write_rng.pop_front(assume::not_empty) = c;
            }

            return initial_rng.without_suffix(write_rng, assume::is_valid);
        }
        return rng;
    }

    template <typename Buf>
    constexpr void normalize_line_endings(const transient<cstrview> string, strcore<Buf>& append_to)
    {
        const usize offset = append_to.size();
        append_to.append(string.get());
        const auto rng = normalize_line_endings(append_to.view(offset).range());
        append_to.truncate(offset + rng.size());
    }

    template <any_strcore Str = str>
    [[nodiscard]] constexpr Str normalize_line_endings(const transient<cstrview> string)
    {
        Str append_to;
        normalize_line_endings(string, append_to);
        return append_to;
    }

    template <typename Buf>
    constexpr void normalize_line_endings_inplace(strcore<Buf>& s)
    {
        s.truncate(normalize_line_endings(s.range()).size());
    }
}
