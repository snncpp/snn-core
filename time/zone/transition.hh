// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

// # Transition

#pragma once

#include "snn-core/core.hh"

namespace snn::time::zone
{
    // ## Classes

    // ### transition

    struct transition final
    {
        // Don't change the member order without checking the generated assembly.
        u8 offset_index : 8;
        i64 when : 56;

        [[nodiscard]] static constexpr bool is_valid(u8, const i64 when) noexcept
        {
            // All indexes are valid.

            // Min-max for 56-bit `i64`.
            return when >= -36028797018963968 && when <= 36028797018963967;
        }
    };
}
