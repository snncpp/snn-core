// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

#include "snn-core/utf8/count.hh"

#include "snn-core/fuzz.hh"

namespace snn
{
    int fuzz(cstrview input)
    {
        const usize count = utf8::count(input).value_or_default();
        snn_assert(count <= input.size());

        return constant::exit::success;
    }
}
