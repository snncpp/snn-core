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

namespace snn::time::zone::db::africa
{
    // clang-format off

    namespace detail::lagos
    {
        inline constexpr array<country::code, 10> country_codes{
            "NG",
            "AO",
            "BJ",
            "CD",
            "CF",
            "CG",
            "CM",
            "GA",
            "GQ",
            "NE",
        };

        inline constexpr array<zone::offset, 4> offsets{{
            {"LMT", 815, false},
            {"GMT", 0, false},
            {"+0030", 1800, false},
            {"WAT", 3600, false},
        }};

        inline constexpr array<zone::transition, 4> transitions{{
            {1, -2035584815},
            {0, -1940889600},
            {2, -1767226415},
            {3, -1588465800},
        }};
    }

    inline constexpr db::entry lagos{"Africa/Lagos",
                                     "West Africa Time",
                                     "+0627+00324",
                                     detail::lagos::country_codes,
                                     detail::lagos::offsets,
                                     detail::lagos::transitions};

    // clang-format on
}
