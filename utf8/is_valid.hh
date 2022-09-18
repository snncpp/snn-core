// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

// # Is valid string

#pragma once

#include "snn-core/range/contiguous.hh"
#include "snn-core/utf8/skip.hh"

namespace snn::utf8
{
    // ## Functions

    // ### is_valid

    [[nodiscard]] constexpr bool is_valid(const transient<cstrview> s) noexcept
    {
        return utf8::skip(s.get().range()).is_empty();
    }
}
