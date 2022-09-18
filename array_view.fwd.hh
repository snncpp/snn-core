// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

// # Array view (forward declare) and `[c]strview` aliases

#pragma once

#include "snn-core/core.hh"

namespace snn
{
    // ## Classes

    // ### array_view

    // Forward declare.

    template <typename, usize = constant::dynamic_count>
    class array_view;

    // ## Aliases

    // ### [c]strview

    using strview  = array_view<char>;
    using cstrview = array_view<const char>;
}
