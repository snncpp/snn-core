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

    namespace detail::ust_nera
    {
        inline constexpr array<country::code, 1> country_codes{
            "RU",
        };

        inline constexpr array<zone::offset, 12> offsets{{
            {"LMT", 34374, false},
            {"+08", 28800, false},
            {"+09", 32400, false},
            {"+11", 39600, false},
            {"+12", 43200, true},
            {"+11", 39600, false},
            {"+12", 43200, true},
            {"+11", 39600, true},
            {"+10", 36000, false},
            {"+12", 43200, false},
            {"+12", 43200, true},
            {"+10", 36000, false},
        }};

        inline constexpr array<zone::transition, 66> transitions{{
            {1, -1579426374},
            {2, -1247558400},
            {4, 354898800},
            {3, 370699200},
            {4, 386427600},
            {3, 402235200},
            {4, 417963600},
            {3, 433771200},
            {4, 449586000},
            {5, 465318000},
            {6, 481042800},
            {5, 496767600},
            {6, 512492400},
            {5, 528217200},
            {6, 543942000},
            {5, 559666800},
            {6, 575391600},
            {5, 591116400},
            {6, 606841200},
            {5, 622566000},
            {6, 638290800},
            {5, 654620400},
            {7, 670345200},
            {8, 686073600},
            {5, 695750400},
            {6, 701794800},
            {5, 717519600},
            {6, 733244400},
            {5, 748969200},
            {6, 764694000},
            {5, 780418800},
            {6, 796143600},
            {5, 811868400},
            {6, 828198000},
            {5, 846342000},
            {6, 859647600},
            {5, 877791600},
            {6, 891097200},
            {5, 909241200},
            {6, 922546800},
            {5, 941295600},
            {6, 953996400},
            {5, 972745200},
            {6, 985446000},
            {5, 1004194800},
            {6, 1017500400},
            {5, 1035644400},
            {6, 1048950000},
            {5, 1067094000},
            {6, 1080399600},
            {5, 1099148400},
            {6, 1111849200},
            {5, 1130598000},
            {6, 1143298800},
            {5, 1162047600},
            {6, 1174748400},
            {5, 1193497200},
            {6, 1206802800},
            {5, 1224946800},
            {6, 1238252400},
            {5, 1256396400},
            {6, 1269702000},
            {5, 1288450800},
            {9, 1301151600},
            {5, 1315828800},
            {8, 1414249200},
        }};
    }

    inline constexpr db::entry ust_nera{"Asia/Ust-Nera",
                                        "MSK+07 - Oymyakonsky",
                                        "+643337+1431336",
                                        detail::ust_nera::country_codes,
                                        detail::ust_nera::offsets,
                                        detail::ust_nera::transitions};

    // clang-format on
}
