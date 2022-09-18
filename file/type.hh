// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

// # Type (enum)

#pragma once

#include "snn-core/core.hh"

namespace snn::file
{
    // ## Enums

    // ### type

    enum class type : u8
    {
        unknown   = 0,
        fifo      = 1,
        character = 2,
        directory = 4,
        block     = 6,
        regular   = 8,
        symlink   = 10,
        socket    = 12,
        whiteout  = 14,
    };
}
