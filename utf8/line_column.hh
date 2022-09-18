// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

// # Byte position to line-column

#pragma once

#include "snn-core/array_view.hh"
#include "snn-core/chr/common.hh"
#include "snn-core/pair/common.hh"
#include "snn-core/range/contiguous.hh"
#include "snn-core/utf8/core.hh"

namespace snn::utf8
{
    // ## Functions

    // ### line_column

    [[nodiscard]] constexpr pair::line_column<usize> line_column(const transient<cstrview> string,
                                                                 const usize byte_position) noexcept
    {
        usize line   = 1;
        usize column = 0;

        auto rng = string.get().range();

        if (byte_position < rng.count())
        {
            const auto initial_rng = rng;

            rng.pop_front_n(byte_position);

            // Include all bytes for the character at byte position.
            if (rng)
            {
                const char c = rng.pop_front(promise::not_empty);
                if (!chr::is_ascii(c))
                {
                    int skip_at_most = 3;
                    while (skip_at_most > 0 && rng.drop_front_if(utf8::is_continuation))
                    {
                        --skip_at_most;
                    }
                }
            }

            rng = initial_rng.without_suffix(rng, promise::is_valid);
        }

        while (rng)
        {
            const char c = rng.front(promise::not_empty);

            if (chr::is_ascii(c))
            {
                rng.drop_front(promise::not_empty);

                if (c == '\n')
                {
                    ++line;
                    column = 0;
                }
                else if (c == '\r')
                {
                    ++line;
                    column = 0;
                    rng.drop_front('\n'); // Skip '\n' in "\r\n" if any.
                }
                else
                {
                    ++column;
                }
            }
            else
            {
                rng.pop_front_codepoint();
                ++column;
            }
        }

        if (column == 0 || byte_position >= string.get().size())
        {
            ++column;
        }

        return {line, column};
    }
}
