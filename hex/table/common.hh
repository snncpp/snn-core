// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

// # Common tables (lower/upper)

#pragma once

#include "snn-core/array.hh"

namespace snn::hex::table
{
    // ## Arrays

    // ### lower

    inline constexpr array<char, 16> lower{'0', '1', '2', '3', '4', '5', '6', '7',
                                           '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'};

    // ### upper

    inline constexpr array<char, 16> upper{'0', '1', '2', '3', '4', '5', '6', '7',
                                           '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};
}
