// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

#include "snn-core/string/normalize_line_endings.hh"

#include "snn-core/fuzz.hh"

namespace snn
{
    int fuzz(cstrview input)
    {
        auto s = string::normalize_line_endings(input);
        snn_assert(s.size() <= input.size());

        return constant::exit::success;
    }
}
