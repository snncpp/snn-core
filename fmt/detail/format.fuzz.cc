// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

#include "snn-core/fmt/format.hh"

#include "snn-core/fuzz.hh"

namespace snn
{
    int fuzz(cstrview input)
    {
        try
        {
            const auto s = fmt::format(input, -42, "abc", 123456);
            ignore_if_unused(s);
        }
        catch (const exception&)
        {
            // Ignore
        }
        return constant::exit::success;
    }
}
