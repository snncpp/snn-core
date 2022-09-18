// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

// # Contiguous range (forward declare) and `[c]strrng` aliases

#pragma once

#include "snn-core/core.hh"

namespace snn::range
{
    // ## Classes

    // ### contiguous

    // Forward declare.

    template <pointer>
    class contiguous;
}

namespace snn
{
    // ## Aliases

    // ### [c]strrng

    using strrng  = range::contiguous<char*>;
    using cstrrng = range::contiguous<const char*>;
}
