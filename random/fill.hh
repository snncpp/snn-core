// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

// # Fill string view with random data

#pragma once

#include "snn-core/array_view.hh"
#include "snn-core/random/detail/source.hh"

namespace snn::random
{
    // ## Functions

    // ### fill

    inline void fill(strview buffer)
    {
        if (buffer)
        {
            random::detail::source::fill(buffer);
        }
    }
}
