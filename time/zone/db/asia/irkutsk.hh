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

    namespace detail::irkutsk
    {
        inline constexpr array<country::code, 1> country_codes{
            "RU",
        };

        inline constexpr array<zone::offset, 12> offsets{{
            {"LMT", 25025, false},
            {"IMT", 25025, false},
            {"+07", 25200, false},
            {"+09", 32400, true},
            {"+08", 28800, false},
            {"+08", 28800, false},
            {"+09", 32400, true},
            {"+08", 28800, true},
            {"+07", 25200, false},
            {"+09", 32400, false},
            {"+09", 32400, true},
            {"+08", 28800, false},
        }};

        inline constexpr array<zone::transition, 67> transitions{{
            {1, -2840165825},
            {2, -1575874625},
            {4, -1247554800},
            {3, 354902400},
            {4, 370710000},
            {3, 386438400},
            {4, 402246000},
            {3, 417974400},
            {4, 433782000},
            {3, 449596800},
            {5, 465328800},
            {6, 481053600},
            {5, 496778400},
            {6, 512503200},
            {5, 528228000},
            {6, 543952800},
            {5, 559677600},
            {6, 575402400},
            {5, 591127200},
            {6, 606852000},
            {5, 622576800},
            {6, 638301600},
            {5, 654631200},
            {7, 670356000},
            {8, 686084400},
            {5, 695761200},
            {6, 701805600},
            {5, 717530400},
            {6, 733255200},
            {5, 748980000},
            {6, 764704800},
            {5, 780429600},
            {6, 796154400},
            {5, 811879200},
            {6, 828208800},
            {5, 846352800},
            {6, 859658400},
            {5, 877802400},
            {6, 891108000},
            {5, 909252000},
            {6, 922557600},
            {5, 941306400},
            {6, 954007200},
            {5, 972756000},
            {6, 985456800},
            {5, 1004205600},
            {6, 1017511200},
            {5, 1035655200},
            {6, 1048960800},
            {5, 1067104800},
            {6, 1080410400},
            {5, 1099159200},
            {6, 1111860000},
            {5, 1130608800},
            {6, 1143309600},
            {5, 1162058400},
            {6, 1174759200},
            {5, 1193508000},
            {6, 1206813600},
            {5, 1224957600},
            {6, 1238263200},
            {5, 1256407200},
            {6, 1269712800},
            {5, 1288461600},
            {9, 1301162400},
            {5, 1414256400},
            {5, 2147483647},
        }};
    }

    inline constexpr db::entry irkutsk{"Asia/Irkutsk",
                                       "MSK+05 - Irkutsk, Buryatia",
                                       "+5216+10420",
                                       detail::irkutsk::country_codes,
                                       detail::irkutsk::offsets,
                                       detail::irkutsk::transitions};

    // clang-format on
}
