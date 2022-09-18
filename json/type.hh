// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

// # Type (enum)

#pragma once

#include "snn-core/core.hh"

namespace snn::json
{
    // ## Enums

    // ### type

    enum class type : u8
    {
        empty = 0,
        array,
        boolean_false,
        boolean_true,
        floating_point,
        signed_integral,
        unsigned_integral,
        null,
        object,
        string,
    };
}
