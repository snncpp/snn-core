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

namespace snn::time::zone::db::europe
{
    // clang-format off

    namespace detail::kaliningrad
    {
        inline constexpr array<country::code, 1> country_codes{
            "RU",
        };

        inline constexpr array<zone::offset, 15> offsets{{
            {"LMT", 4920, false},
            {"CEST", 7200, true},
            {"CET", 3600, false},
            {"CEST", 7200, true},
            {"CET", 3600, false},
            {"EEST", 10800, true},
            {"EET", 7200, false},
            {"MSD", 14400, true},
            {"MSK", 10800, false},
            {"MSK", 10800, false},
            {"MSD", 14400, true},
            {"EEST", 10800, true},
            {"EET", 7200, false},
            {"+03", 10800, false},
            {"EET", 7200, false},
        }};

        inline constexpr array<zone::transition, 80> transitions{{
            {2, -2422056120},
            {1, -1693706400},
            {2, -1680483600},
            {3, -1663455600},
            {4, -1650150000},
            {3, -1632006000},
            {4, -1618700400},
            {3, -938905200},
            {4, -857257200},
            {3, -844556400},
            {4, -828226800},
            {3, -812502000},
            {4, -796777200},
            {3, -781052400},
            {6, -780372000},
            {5, -778730400},
            {6, -762663600},
            {8, -749095200},
            {7, 354920400},
            {8, 370728000},
            {7, 386456400},
            {8, 402264000},
            {7, 417992400},
            {8, 433800000},
            {7, 449614800},
            {9, 465346800},
            {10, 481071600},
            {9, 496796400},
            {10, 512521200},
            {9, 528246000},
            {10, 543970800},
            {9, 559695600},
            {10, 575420400},
            {9, 591145200},
            {11, 606870000},
            {12, 622598400},
            {11, 638323200},
            {12, 654652800},
            {11, 670377600},
            {12, 686102400},
            {11, 701827200},
            {12, 717552000},
            {11, 733276800},
            {12, 749001600},
            {11, 764726400},
            {12, 780451200},
            {11, 796176000},
            {12, 811900800},
            {11, 828230400},
            {12, 846374400},
            {11, 859680000},
            {12, 877824000},
            {11, 891129600},
            {12, 909273600},
            {11, 922579200},
            {12, 941328000},
            {11, 954028800},
            {12, 972777600},
            {11, 985478400},
            {12, 1004227200},
            {11, 1017532800},
            {12, 1035676800},
            {11, 1048982400},
            {12, 1067126400},
            {11, 1080432000},
            {12, 1099180800},
            {11, 1111881600},
            {12, 1130630400},
            {11, 1143331200},
            {12, 1162080000},
            {11, 1174780800},
            {12, 1193529600},
            {11, 1206835200},
            {12, 1224979200},
            {11, 1238284800},
            {12, 1256428800},
            {11, 1269734400},
            {12, 1288483200},
            {13, 1301184000},
            {12, 1414278000},
        }};
    }

    inline constexpr db::entry kaliningrad{"Europe/Kaliningrad",
                                           "MSK-01 - Kaliningrad",
                                           "+5443+02030",
                                           detail::kaliningrad::country_codes,
                                           detail::kaliningrad::offsets,
                                           detail::kaliningrad::transitions};

    // clang-format on
}
