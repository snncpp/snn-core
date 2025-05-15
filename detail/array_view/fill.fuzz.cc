// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

#include "snn-core/array_view.hh"

#include "snn-core/fuzz.hh"
#include "snn-core/strcore.hh"
#include "snn-core/string/range/chunk.hh"

namespace snn::app
{
    namespace
    {
        template <typename Str>
        void fuzz_fill(const cstrview input)
        {
            const usize fill_string_pos  = to_byte(input.at(0).value_or(0));
            const usize fill_string_size = to_byte(input.at(1).value_or(0));

            Str buf{input};

            // Must handle overlapping memory.
            cstrview fill_string = buf.view(fill_string_pos, fill_string_size);

            if (fill_string.is_empty())
            {
                fill_string = "abc";
            }

            // If "fill_string" points into "buf" its data can be overwritten.
            const Str original_fill_string = fill_string;

            const usize cap = buf.capacity();

            buf.resize_for_overwrite(cap).fill(fill_string);

            snn_assert(buf.capacity() == cap);
            snn_assert(buf.size() == cap);

            for (const auto s : string::range::chunk{buf, fill_string.size()})
            {
                snn_assert(s == original_fill_string.view(0, s.size()));
            }
        }
    }
}

namespace snn
{
    int fuzz(cstrview input)
    {
        app::fuzz_fill<str>(input);
        app::fuzz_fill<strbuf>(input);

        return constant::exit::success;
    }
}
