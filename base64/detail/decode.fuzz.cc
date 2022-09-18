// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

#include "snn-core/base64/decode.hh"

#include "snn-core/fuzz.hh"

namespace snn
{
    int fuzz(cstrview input)
    {
        const auto r1 = base64::decode(input, base64::padding_required);
        ignore_if_unused(r1);

        const auto r2 = base64::decode(input, base64::padding_optional);
        ignore_if_unused(r2);

        return constant::exit::success;
    }
}
