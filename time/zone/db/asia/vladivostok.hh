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

    namespace detail::vladivostok
    {
        inline constexpr array<country::code, 1> country_codes{
            "RU",
        };

        inline constexpr array<zone::offset, 11> offsets{{
            {"LMT", 31651, false},
            {"+09", 32400, false},
            {"+11", 39600, true},
            {"+10", 36000, false},
            {"+10", 36000, false},
            {"+11", 39600, true},
            {"+10", 36000, true},
            {"+09", 32400, false},
            {"+11", 39600, false},
            {"+11", 39600, true},
            {"+10", 36000, false},
        }};

        inline constexpr array<zone::transition, 65> transitions{{
            {1, -1487321251},
            {3, -1247562000},
            {2, 354895200},
            {3, 370702800},
            {2, 386431200},
            {3, 402238800},
            {2, 417967200},
            {3, 433774800},
            {2, 449589600},
            {4, 465321600},
            {5, 481046400},
            {4, 496771200},
            {5, 512496000},
            {4, 528220800},
            {5, 543945600},
            {4, 559670400},
            {5, 575395200},
            {4, 591120000},
            {5, 606844800},
            {4, 622569600},
            {5, 638294400},
            {4, 654624000},
            {6, 670348800},
            {7, 686077200},
            {4, 695754000},
            {5, 701798400},
            {4, 717523200},
            {5, 733248000},
            {4, 748972800},
            {5, 764697600},
            {4, 780422400},
            {5, 796147200},
            {4, 811872000},
            {5, 828201600},
            {4, 846345600},
            {5, 859651200},
            {4, 877795200},
            {5, 891100800},
            {4, 909244800},
            {5, 922550400},
            {4, 941299200},
            {5, 954000000},
            {4, 972748800},
            {5, 985449600},
            {4, 1004198400},
            {5, 1017504000},
            {4, 1035648000},
            {5, 1048953600},
            {4, 1067097600},
            {5, 1080403200},
            {4, 1099152000},
            {5, 1111852800},
            {4, 1130601600},
            {5, 1143302400},
            {4, 1162051200},
            {5, 1174752000},
            {4, 1193500800},
            {5, 1206806400},
            {4, 1224950400},
            {5, 1238256000},
            {4, 1256400000},
            {5, 1269705600},
            {4, 1288454400},
            {8, 1301155200},
            {4, 1414249200},
        }};
    }

    inline constexpr db::entry vladivostok{"Asia/Vladivostok",
                                           "MSK+07 - Amur River",
                                           "+4310+13156",
                                           detail::vladivostok::country_codes,
                                           detail::vladivostok::offsets,
                                           detail::vladivostok::transitions};

    // clang-format on
}
