// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

#include "snn-core/time/parse.hh"

#include "snn-core/fuzz.hh"

namespace snn
{
    int fuzz(cstrview input)
    {
        const auto r = time::parse(input);
        ignore_if_unused(r);

        return constant::exit::success;
    }
}
