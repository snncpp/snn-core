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

namespace snn::time::zone::db::asia
{
    // clang-format off

    namespace detail::hong_kong
    {
        inline constexpr array<country::code, 1> country_codes{
            "HK",
        };

        inline constexpr array<zone::offset, 8> offsets{{
            {"LMT", 27402, false},
            {"HKT", 28800, false},
            {"HKST", 32400, true},
            {"HKWT", 30600, true},
            {"JST", 32400, false},
            {"HKT", 28800, false},
            {"HKST", 32400, true},
            {"HKT", 28800, false},
        }};

        inline constexpr array<zone::transition, 69> transitions{{
            {1, -2056690800},
            {2, -900910800},
            {3, -891579600},
            {4, -884248200},
            {7, -761209200},
            {2, -747907200},
            {5, -728541000},
            {6, -717049800},
            {5, -697091400},
            {6, -683785800},
            {5, -668061000},
            {2, -654755400},
            {5, -636611400},
            {2, -623305800},
            {5, -605161800},
            {2, -591856200},
            {5, -573712200},
            {2, -559801800},
            {5, -541657800},
            {2, -528352200},
            {7, -510211800},
            {2, -498112200},
            {7, -478762200},
            {2, -466662600},
            {7, -446707800},
            {2, -435213000},
            {7, -415258200},
            {2, -403158600},
            {7, -383808600},
            {2, -371709000},
            {7, -352359000},
            {2, -340259400},
            {7, -320909400},
            {2, -308809800},
            {7, -288855000},
            {2, -277360200},
            {7, -257405400},
            {2, -245910600},
            {7, -225955800},
            {2, -213856200},
            {7, -194506200},
            {2, -182406600},
            {7, -163056600},
            {2, -148537800},
            {7, -132816600},
            {2, -117088200},
            {7, -101367000},
            {2, -85638600},
            {7, -69312600},
            {2, -53584200},
            {7, -37863000},
            {2, -22134600},
            {7, -6413400},
            {2, 9315000},
            {7, 25036200},
            {2, 40764600},
            {7, 56485800},
            {2, 72214200},
            {7, 88540200},
            {2, 104268600},
            {7, 119989800},
            {2, 126041400},
            {7, 151439400},
            {2, 167167800},
            {7, 182889000},
            {2, 198617400},
            {7, 214338600},
            {2, 295385400},
            {7, 309292200},
        }};
    }

    inline constexpr db::entry hong_kong{"Asia/Hong_Kong",
                                         "",
                                         "+2217+11409",
                                         detail::hong_kong::country_codes,
                                         detail::hong_kong::offsets,
                                         detail::hong_kong::transitions};

    // clang-format on
}
