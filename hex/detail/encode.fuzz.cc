// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

#include "snn-core/hex/encode.hh"

#include "snn-core/fuzz.hh"

namespace snn
{
    int fuzz(cstrview input)
    {
        const auto s = hex::encode(input);
        ignore_if_unused(s);

        return constant::exit::success;
    }
}
