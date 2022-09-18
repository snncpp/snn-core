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

    namespace detail::minsk
    {
        inline constexpr array<country::code, 1> country_codes{
            "BY",
        };

        inline constexpr array<zone::offset, 13> offsets{{
            {"LMT", 6616, false},
            {"MMT", 6600, false},
            {"EET", 7200, false},
            {"MSK", 10800, false},
            {"CET", 3600, false},
            {"CEST", 7200, true},
            {"CEST", 7200, true},
            {"MSD", 14400, true},
            {"MSK", 10800, false},
            {"MSD", 14400, true},
            {"EEST", 10800, true},
            {"EET", 7200, false},
            {"+03", 10800, false},
        }};

        inline constexpr array<zone::transition, 69> transitions{{
            {1, -2840147416},
            {2, -1441158600},
            {3, -1247536800},
            {6, -899780400},
            {4, -857257200},
            {5, -844556400},
            {4, -828226800},
            {5, -812502000},
            {3, -804650400},
            {7, 354920400},
            {3, 370728000},
            {7, 386456400},
            {3, 402264000},
            {7, 417992400},
            {3, 433800000},
            {7, 449614800},
            {8, 465346800},
            {9, 481071600},
            {8, 496796400},
            {9, 512521200},
            {8, 528246000},
            {9, 543970800},
            {8, 559695600},
            {9, 575420400},
            {8, 591145200},
            {9, 606870000},
            {8, 622594800},
            {10, 670374000},
            {11, 686102400},
            {10, 701827200},
            {11, 717552000},
            {10, 733276800},
            {11, 749001600},
            {10, 764726400},
            {11, 780451200},
            {10, 796176000},
            {11, 811900800},
            {10, 828230400},
            {11, 846374400},
            {10, 859680000},
            {11, 877824000},
            {10, 891129600},
            {11, 909273600},
            {10, 922579200},
            {11, 941328000},
            {10, 954028800},
            {11, 972777600},
            {10, 985478400},
            {11, 1004227200},
            {10, 1017532800},
            {11, 1035676800},
            {10, 1048982400},
            {11, 1067126400},
            {10, 1080432000},
            {11, 1099180800},
            {10, 1111881600},
            {11, 1130630400},
            {10, 1143331200},
            {11, 1162080000},
            {10, 1174780800},
            {11, 1193529600},
            {10, 1206835200},
            {11, 1224979200},
            {10, 1238284800},
            {11, 1256428800},
            {10, 1269734400},
            {11, 1288483200},
            {12, 1301184000},
            {12, 2147483647},
        }};
    }

    inline constexpr db::entry minsk{"Europe/Minsk",
                                     "",
                                     "+5354+02734",
                                     detail::minsk::country_codes,
                                     detail::minsk::offsets,
                                     detail::minsk::transitions};

    // clang-format on
}
