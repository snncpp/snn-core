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

    namespace detail::macau
    {
        inline constexpr array<country::code, 1> country_codes{
            "MO",
        };

        inline constexpr array<zone::offset, 7> offsets{{
            {"LMT", 27250, false},
            {"CST", 28800, false},
            {"+10", 36000, true},
            {"+09", 32400, false},
            {"CDT", 32400, true},
            {"CST", 28800, false},
            {"CDT", 32400, true},
        }};

        inline constexpr array<zone::transition, 71> transitions{{
            {1, -2056692850},
            {3, -884509200},
            {2, -873280800},
            {3, -855918000},
            {2, -841744800},
            {3, -828529200},
            {1, -765363600},
            {4, -747046800},
            {5, -733827600},
            {4, -716461200},
            {5, -697021200},
            {4, -683715600},
            {5, -667990800},
            {4, -654771600},
            {5, -636627600},
            {4, -623322000},
            {5, -605178000},
            {4, -591872400},
            {5, -573642000},
            {4, -559818000},
            {5, -541674000},
            {4, -528368400},
            {5, -510224400},
            {4, -498128400},
            {5, -478774800},
            {4, -466678800},
            {5, -446720400},
            {4, -435229200},
            {1, -415258200},
            {6, -403158600},
            {1, -383808600},
            {6, -371709000},
            {1, -352359000},
            {6, -340259400},
            {1, -320909400},
            {6, -308809800},
            {1, -288855000},
            {6, -277360200},
            {1, -257405400},
            {6, -245910600},
            {1, -225955800},
            {6, -213856200},
            {1, -194506200},
            {6, -182406600},
            {1, -163056600},
            {6, -148537800},
            {1, -132820200},
            {6, -117088200},
            {1, -101370600},
            {6, -85638600},
            {1, -69312600},
            {6, -53584200},
            {1, -37863000},
            {6, -22134600},
            {1, -6413400},
            {6, 9315000},
            {1, 25036200},
            {6, 40764600},
            {1, 56485800},
            {6, 72214200},
            {1, 88540200},
            {6, 104268600},
            {1, 119989800},
            {6, 126041400},
            {1, 151439400},
            {6, 167167800},
            {1, 182889000},
            {6, 198617400},
            {1, 214338600},
            {6, 295385400},
            {1, 309292200},
        }};
    }

    inline constexpr db::entry macau{"Asia/Macau",
                                     "",
                                     "+221150+1133230",
                                     detail::macau::country_codes,
                                     detail::macau::offsets,
                                     detail::macau::transitions};

    // clang-format on
}
