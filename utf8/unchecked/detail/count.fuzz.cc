// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

#include "snn-core/utf8/unchecked/count.hh"

#include "snn-core/fuzz.hh"

namespace snn
{
    int fuzz(cstrview input)
    {
        const usize count = utf8::unchecked::count(input);
        snn_assert(count <= input.size());

        return constant::exit::success;
    }
}
