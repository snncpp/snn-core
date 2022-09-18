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

    namespace detail::simferopol
    {
        inline constexpr array<country::code, 2> country_codes{
            "RU",
            "UA",
        };

        inline constexpr array<zone::offset, 16> offsets{{
            {"LMT", 8184, false},
            {"SMT", 8160, false},
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
            {"EEST", 10800, true},
            {"EET", 7200, false},
            {"MSK", 14400, false},
            {"MSK", 10800, false},
        }};

        inline constexpr array<zone::transition, 75> transitions{{
            {1, -2840148984},
            {2, -1441160160},
            {3, -1247536800},
            {6, -888894000},
            {4, -857257200},
            {5, -844556400},
            {4, -828226800},
            {5, -812502000},
            {3, -811648800},
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
            {2, 646786800},
            {10, 701827200},
            {11, 717552000},
            {10, 733276800},
            {11, 749001600},
            {10, 764726400},
            {7, 767739600},
            {8, 780447600},
            {9, 796172400},
            {8, 811897200},
            {9, 828219600},
            {8, 846374400},
            {12, 859683600},
            {13, 877827600},
            {12, 891133200},
            {13, 909277200},
            {12, 922582800},
            {13, 941331600},
            {12, 954032400},
            {13, 972781200},
            {12, 985482000},
            {13, 1004230800},
            {12, 1017536400},
            {13, 1035680400},
            {12, 1048986000},
            {13, 1067130000},
            {12, 1080435600},
            {13, 1099184400},
            {12, 1111885200},
            {13, 1130634000},
            {12, 1143334800},
            {13, 1162083600},
            {12, 1174784400},
            {13, 1193533200},
            {12, 1206838800},
            {13, 1224982800},
            {12, 1238288400},
            {13, 1256432400},
            {12, 1269738000},
            {13, 1288486800},
            {12, 1301187600},
            {13, 1319936400},
            {12, 1332637200},
            {13, 1351386000},
            {12, 1364691600},
            {13, 1382835600},
            {14, 1396137600},
            {8, 1414274400},
        }};
    }

    inline constexpr db::entry simferopol{"Europe/Simferopol",
                                          "Crimea",
                                          "+4457+03406",
                                          detail::simferopol::country_codes,
                                          detail::simferopol::offsets,
                                          detail::simferopol::transitions};

    // clang-format on
}
