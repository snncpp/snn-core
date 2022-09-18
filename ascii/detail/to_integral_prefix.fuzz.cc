// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

#include "snn-core/ascii/to_integral_prefix.hh"

#include "snn-core/fuzz.hh"
#include "snn-core/range/contiguous.hh"

namespace snn
{
    int fuzz(cstrview input)
    {
        auto p = ascii::to_integral_prefix<i32>(input.range(), ascii::leading_zeros::allow);
        ignore_if_unused(p);

        return constant::exit::success;
    }
}
