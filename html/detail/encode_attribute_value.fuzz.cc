// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

#include "snn-core/html/encode.hh"

#include "snn-core/fuzz.hh"

namespace snn
{
    int fuzz(cstrview input)
    {
        // UTF-8 validation is disabled when fuzzing.
        const auto s = html::encode_attribute_value(input, assume::is_utf8);
        ignore_if_unused(s);

        return constant::exit::success;
    }
}
