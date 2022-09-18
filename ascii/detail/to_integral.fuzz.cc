// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

#include "snn-core/ascii/to_integral.hh"

#include "snn-core/fuzz.hh"
#include "snn-core/range/contiguous.hh"

namespace snn
{
    int fuzz(cstrview input)
    {
        auto opt = ascii::to_integral<i32>(input.range());
        ignore_if_unused(opt);

        return constant::exit::success;
    }
}
