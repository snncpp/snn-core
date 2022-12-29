// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

// # Random boolean

#pragma once

#include "snn-core/random/number.hh"

namespace snn::random
{
    // ## Functions

    // ### boolean

    [[nodiscard]] inline bool boolean()
    {
        const auto i = random::number<u32>();
        return static_cast<bool>(i & 1u);
    }
}
