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

    namespace detail::yekaterinburg
    {
        inline constexpr array<country::code, 1> country_codes{
            "RU",
        };

        inline constexpr array<zone::offset, 12> offsets{{
            {"LMT", 14553, false},
            {"PMT", 13505, false},
            {"+04", 14400, false},
            {"+06", 21600, true},
            {"+05", 18000, false},
            {"+05", 18000, false},
            {"+06", 21600, true},
            {"+05", 18000, true},
            {"+04", 14400, false},
            {"+06", 21600, false},
            {"+06", 21600, true},
            {"+05", 18000, false},
        }};

        inline constexpr array<zone::transition, 67> transitions{{
            {1, -1688270553},
            {2, -1592610305},
            {4, -1247544000},
            {3, 354913200},
            {4, 370720800},
            {3, 386449200},
            {4, 402256800},
            {3, 417985200},
            {4, 433792800},
            {3, 449607600},
            {5, 465339600},
            {6, 481064400},
            {5, 496789200},
            {6, 512514000},
            {5, 528238800},
            {6, 543963600},
            {5, 559688400},
            {6, 575413200},
            {5, 591138000},
            {6, 606862800},
            {5, 622587600},
            {6, 638312400},
            {5, 654642000},
            {7, 670366800},
            {8, 686095200},
            {5, 695772000},
            {6, 701816400},
            {5, 717541200},
            {6, 733266000},
            {5, 748990800},
            {6, 764715600},
            {5, 780440400},
            {6, 796165200},
            {5, 811890000},
            {6, 828219600},
            {5, 846363600},
            {6, 859669200},
            {5, 877813200},
            {6, 891118800},
            {5, 909262800},
            {6, 922568400},
            {5, 941317200},
            {6, 954018000},
            {5, 972766800},
            {6, 985467600},
            {5, 1004216400},
            {6, 1017522000},
            {5, 1035666000},
            {6, 1048971600},
            {5, 1067115600},
            {6, 1080421200},
            {5, 1099170000},
            {6, 1111870800},
            {5, 1130619600},
            {6, 1143320400},
            {5, 1162069200},
            {6, 1174770000},
            {5, 1193518800},
            {6, 1206824400},
            {5, 1224968400},
            {6, 1238274000},
            {5, 1256418000},
            {6, 1269723600},
            {5, 1288472400},
            {9, 1301173200},
            {5, 1414267200},
            {5, 2147483647},
        }};
    }

    inline constexpr db::entry yekaterinburg{"Asia/Yekaterinburg",
                                             "MSK+02 - Urals",
                                             "+5651+06036",
                                             detail::yekaterinburg::country_codes,
                                             detail::yekaterinburg::offsets,
                                             detail::yekaterinburg::transitions};

    // clang-format on
}
