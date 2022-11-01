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

    namespace detail::magadan
    {
        inline constexpr array<country::code, 1> country_codes{
            "RU",
        };

        inline constexpr array<zone::offset, 11> offsets{{
            {"LMT", 36192, false},
            {"+10", 36000, false},
            {"+12", 43200, true},
            {"+11", 39600, false},
            {"+11", 39600, false},
            {"+12", 43200, true},
            {"+11", 39600, true},
            {"+10", 36000, false},
            {"+12", 43200, false},
            {"+12", 43200, true},
            {"+11", 39600, false},
        }};

        inline constexpr array<zone::transition, 66> transitions{{
            {1, -1441188192},
            {3, -1247565600},
            {2, 354891600},
            {3, 370699200},
            {2, 386427600},
            {3, 402235200},
            {2, 417963600},
            {3, 433771200},
            {2, 449586000},
            {4, 465318000},
            {5, 481042800},
            {4, 496767600},
            {5, 512492400},
            {4, 528217200},
            {5, 543942000},
            {4, 559666800},
            {5, 575391600},
            {4, 591116400},
            {5, 606841200},
            {4, 622566000},
            {5, 638290800},
            {4, 654620400},
            {6, 670345200},
            {7, 686073600},
            {4, 695750400},
            {5, 701794800},
            {4, 717519600},
            {5, 733244400},
            {4, 748969200},
            {5, 764694000},
            {4, 780418800},
            {5, 796143600},
            {4, 811868400},
            {5, 828198000},
            {4, 846342000},
            {5, 859647600},
            {4, 877791600},
            {5, 891097200},
            {4, 909241200},
            {5, 922546800},
            {4, 941295600},
            {5, 953996400},
            {4, 972745200},
            {5, 985446000},
            {4, 1004194800},
            {5, 1017500400},
            {4, 1035644400},
            {5, 1048950000},
            {4, 1067094000},
            {5, 1080399600},
            {4, 1099148400},
            {5, 1111849200},
            {4, 1130598000},
            {5, 1143298800},
            {4, 1162047600},
            {5, 1174748400},
            {4, 1193497200},
            {5, 1206802800},
            {4, 1224946800},
            {5, 1238252400},
            {4, 1256396400},
            {5, 1269702000},
            {4, 1288450800},
            {8, 1301151600},
            {7, 1414245600},
            {4, 1461427200},
        }};
    }

    inline constexpr db::entry magadan{"Asia/Magadan",
                                       "MSK+08 - Magadan",
                                       "+5934+15048",
                                       detail::magadan::country_codes,
                                       detail::magadan::offsets,
                                       detail::magadan::transitions};

    // clang-format on
}
