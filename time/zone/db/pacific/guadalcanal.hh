// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

// This file is generated, DO NOT EDIT MANUALLY.

// IANA Time Zone Database (public domain).

#pragma once

#include "snn-core/array.hh"
#include "snn-core/country/code.hh"
#include "snn-core/time/zone/offset.hh"
#include "snn-core/time/zone/transition.hh"
#include "snn-core/time/zone/db/entry.hh"

namespace snn::time::zone::db::pacific
{
    // clang-format off

    namespace detail::guadalcanal
    {
        inline constexpr array<country::code, 2> country_codes{
            "SB",
            "FM",
        };

        inline constexpr array<zone::offset, 2> offsets{{
            {"LMT", 38388, false},
            {"+11", 39600, false},
        }};

        inline constexpr array<zone::transition, 2> transitions{{
            {1, -1806748788},
            {1, 2147483647},
        }};
    }

    inline constexpr db::entry guadalcanal{"Pacific/Guadalcanal",
                                           "Solomons, Pohnpei",
                                           "-0932+16012",
                                           detail::guadalcanal::country_codes,
                                           detail::guadalcanal::offsets,
                                           detail::guadalcanal::transitions};

    // clang-format on
}
