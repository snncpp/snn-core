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

    namespace detail::bahia
    {
        inline constexpr array<country::code, 1> country_codes{
            "BR",
        };

        inline constexpr array<zone::offset, 3> offsets{{
            {"LMT", -9244, false},
            {"-02", -7200, true},
            {"-03", -10800, false},
        }};

        inline constexpr array<zone::transition, 61> transitions{{
            {2, -1767216356},
            {1, -1206957600},
            {2, -1191362400},
            {1, -1175374800},
            {2, -1159826400},
            {1, -633819600},
            {2, -622069200},
            {1, -602283600},
            {2, -591832800},
            {1, -570747600},
            {2, -560210400},
            {1, -539125200},
            {2, -531352800},
            {1, -191365200},
            {2, -184197600},
            {1, -155163600},
            {2, -150069600},
            {1, -128898000},
            {2, -121125600},
            {1, -99954000},
            {2, -89589600},
            {1, -68418000},
            {2, -57967200},
            {1, 499748400},
            {2, 511236000},
            {1, 530593200},
            {2, 540266400},
            {1, 562129200},
            {2, 571197600},
            {1, 592974000},
            {2, 602042400},
            {1, 624423600},
            {2, 634701600},
            {1, 656478000},
            {2, 666756000},
            {1, 687927600},
            {2, 697600800},
            {1, 719982000},
            {2, 728445600},
            {1, 750826800},
            {2, 761709600},
            {1, 782276400},
            {2, 793159200},
            {1, 813726000},
            {2, 824004000},
            {1, 844570800},
            {2, 856058400},
            {1, 876106800},
            {2, 888717600},
            {1, 908074800},
            {2, 919562400},
            {1, 938919600},
            {2, 951616800},
            {1, 970974000},
            {2, 982461600},
            {1, 1003028400},
            {2, 1013911200},
            {1, 1036292400},
            {2, 1045360800},
            {1, 1318734000},
            {2, 1330221600},
        }};
    }

    inline constexpr db::entry bahia{"America/Bahia",
                                     "Bahia",
                                     "-1259-03831",
                                     detail::bahia::country_codes,
                                     detail::bahia::offsets,
                                     detail::bahia::transitions};

    // clang-format on
}
