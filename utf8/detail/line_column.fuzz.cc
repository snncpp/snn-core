// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

#include "snn-core/utf8/line_column.hh"

#include "snn-core/fuzz.hh"

namespace snn
{
    int fuzz(cstrview input)
    {
        if (input)
        {
            const usize byte_position = to_byte(input.front(promise::not_empty));
            input.drop_front_n(1);

            const auto p    = utf8::line_column(input, byte_position);
            const usize max = input.size() + 1;
            snn_assert(p.line >= 1 && p.line <= max);
            snn_assert(p.column >= 1 && p.column <= max);
        }

        return constant::exit::success;
    }
}
