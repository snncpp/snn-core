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

        inline constexpr array<zone::offset, 7> offsets{{
            {"LMT", -26632, false},
            {"MST", -25200, false},
            {"CST", -21600, false},
            {"MDT", -21600, true},
            {"MST", -25200, false},
            {"PST", -28800, false},
            {"MST", -25200, false},
        }};

        inline constexpr array<zone::transition, 15> transitions{{
            {1, -1514739600},
            {2, -1343066400},
            {4, -1234807200},
            {3, -1220292000},
            {4, -1207159200},
            {2, -1191344400},
            {4, -873828000},
            {5, -661539600},
            {4, 28800},
            {3, 828867600},
            {4, 846403200},
            {3, 860317200},
            {4, 877852800},
            {3, 891766800},
            {4, 909302400},
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
