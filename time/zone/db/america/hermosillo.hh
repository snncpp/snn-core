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

namespace snn::time::zone::db::america
{
    // clang-format off

    namespace detail::hermosillo
    {
        inline constexpr array<country::code, 1> country_codes{
            "MX",
        };

        inline constexpr array<zone::offset, 6> offsets{{
            {"LMT", -26632, false},
            {"MST", -25200, false},
            {"CST", -21600, false},
            {"PST", -28800, false},
            {"MDT", -21600, true},
            {"MST", -25200, false},
        }};

        inline constexpr array<zone::transition, 15> transitions{{
            {1, -1514739600},
            {2, -1343066400},
            {1, -1234807200},
            {2, -1220292000},
            {1, -1207159200},
            {2, -1191344400},
            {1, -873828000},
            {3, -661539600},
            {1, 28800},
            {4, 828867600},
            {1, 846403200},
            {4, 860317200},
            {1, 877852800},
            {4, 891766800},
            {1, 909302400},
        }};
    }

    inline constexpr db::entry hermosillo{"America/Hermosillo",
                                          "Mountain Standard Time - Sonora",
                                          "+2904-11058",
                                          detail::hermosillo::country_codes,
                                          detail::hermosillo::offsets,
                                          detail::hermosillo::transitions};

    // clang-format on
}
