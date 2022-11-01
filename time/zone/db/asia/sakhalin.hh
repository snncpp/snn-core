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

    namespace detail::sakhalin
    {
        inline constexpr array<country::code, 1> country_codes{
            "RU",
        };

        inline constexpr array<zone::offset, 9> offsets{{
            {"LMT", 34248, false},
            {"+09", 32400, false},
            {"+12", 43200, true},
            {"+11", 39600, false},
            {"+11", 39600, false},
            {"+12", 43200, true},
            {"+11", 39600, true},
            {"+10", 36000, false},
            {"+11", 39600, false},
        }};

        inline constexpr array<zone::transition, 66> transitions{{
            {1, -2031039048},
            {3, -768560400},
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
            {6, 859647600},
            {7, 877795200},
            {6, 891100800},
            {7, 909244800},
            {6, 922550400},
            {7, 941299200},
            {6, 954000000},
            {7, 972748800},
            {6, 985449600},
            {7, 1004198400},
            {6, 1017504000},
            {7, 1035648000},
            {6, 1048953600},
            {7, 1067097600},
            {6, 1080403200},
            {7, 1099152000},
            {6, 1111852800},
            {7, 1130601600},
            {6, 1143302400},
            {7, 1162051200},
            {6, 1174752000},
            {7, 1193500800},
            {6, 1206806400},
            {7, 1224950400},
            {6, 1238256000},
            {7, 1256400000},
            {6, 1269705600},
            {7, 1288454400},
            {4, 1301155200},
            {7, 1414249200},
            {4, 1459008000},
        }};
    }

    inline constexpr db::entry sakhalin{"Asia/Sakhalin",
                                        "MSK+08 - Sakhalin Island",
                                        "+4658+14242",
                                        detail::sakhalin::country_codes,
                                        detail::sakhalin::offsets,
                                        detail::sakhalin::transitions};

    // clang-format on
}
