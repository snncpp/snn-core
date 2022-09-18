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

    namespace detail::moscow
    {
        inline constexpr array<country::code, 1> country_codes{
            "RU",
        };

        inline constexpr array<zone::offset, 17> offsets{{
            {"LMT", 9017, false},
            {"MMT", 9017, false},
            {"MST", 12679, true},
            {"MMT", 9079, false},
            {"MDST", 16279, true},
            {"MSD", 14400, true},
            {"MSK", 10800, false},
            {"MSD", 14400, true},
            {"+05", 18000, true},
            {"EET", 7200, false},
            {"MSK", 10800, false},
            {"MSD", 14400, true},
            {"EEST", 10800, true},
            {"EET", 7200, false},
            {"MSK", 14400, false},
            {"MSD", 14400, true},
            {"MSK", 10800, false},
        }};

        inline constexpr array<zone::transition, 78> transitions{{
            {1, -2840149817},
            {3, -1688265017},
            {2, -1656819079},
            {3, -1641353479},
            {4, -1627965079},
            {2, -1618716679},
            {4, -1596429079},
            {5, -1593820800},
            {6, -1589860800},
            {7, -1542427200},
            {8, -1539493200},
            {7, -1525323600},
            {6, -1522728000},
            {9, -1491188400},
            {6, -1247536800},
            {7, 354920400},
            {6, 370728000},
            {7, 386456400},
            {6, 402264000},
            {7, 417992400},
            {6, 433800000},
            {7, 449614800},
            {10, 465346800},
            {11, 481071600},
            {10, 496796400},
            {11, 512521200},
            {10, 528246000},
            {11, 543970800},
            {10, 559695600},
            {11, 575420400},
            {10, 591145200},
            {11, 606870000},
            {10, 622594800},
            {11, 638319600},
            {10, 654649200},
            {12, 670374000},
            {13, 686102400},
            {10, 695779200},
            {11, 701823600},
            {10, 717548400},
            {11, 733273200},
            {10, 748998000},
            {11, 764722800},
            {10, 780447600},
            {11, 796172400},
            {10, 811897200},
            {11, 828226800},
            {10, 846370800},
            {11, 859676400},
            {10, 877820400},
            {11, 891126000},
            {10, 909270000},
            {11, 922575600},
            {10, 941324400},
            {11, 954025200},
            {10, 972774000},
            {11, 985474800},
            {10, 1004223600},
            {11, 1017529200},
            {10, 1035673200},
            {11, 1048978800},
            {10, 1067122800},
            {11, 1080428400},
            {10, 1099177200},
            {11, 1111878000},
            {10, 1130626800},
            {11, 1143327600},
            {10, 1162076400},
            {11, 1174777200},
            {10, 1193526000},
            {11, 1206831600},
            {10, 1224975600},
            {11, 1238281200},
            {10, 1256425200},
            {11, 1269730800},
            {10, 1288479600},
            {14, 1301180400},
            {10, 1414274400},
        }};
    }

    inline constexpr db::entry moscow{"Europe/Moscow",
                                      "MSK+00 - Moscow area",
                                      "+554521+0373704",
                                      detail::moscow::country_codes,
                                      detail::moscow::offsets,
                                      detail::moscow::transitions};

    // clang-format on
}
