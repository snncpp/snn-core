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

    namespace detail::chita
    {
        inline constexpr array<country::code, 1> country_codes{
            "RU",
        };

        inline constexpr array<zone::offset, 11> offsets{{
            {"LMT", 27232, false},
            {"+08", 28800, false},
            {"+10", 36000, true},
            {"+09", 32400, false},
            {"+09", 32400, false},
            {"+10", 36000, true},
            {"+09", 32400, true},
            {"+08", 28800, false},
            {"+10", 36000, false},
            {"+10", 36000, true},
            {"+09", 32400, false},
        }};

        inline constexpr array<zone::transition, 66> transitions{{
            {1, -1579419232},
            {3, -1247558400},
            {2, 354898800},
            {3, 370706400},
            {2, 386434800},
            {3, 402242400},
            {2, 417970800},
            {3, 433778400},
            {2, 449593200},
            {4, 465325200},
            {5, 481050000},
            {4, 496774800},
            {5, 512499600},
            {4, 528224400},
            {5, 543949200},
            {4, 559674000},
            {5, 575398800},
            {4, 591123600},
            {5, 606848400},
            {4, 622573200},
            {5, 638298000},
            {4, 654627600},
            {6, 670352400},
            {7, 686080800},
            {4, 695757600},
            {5, 701802000},
            {4, 717526800},
            {5, 733251600},
            {4, 748976400},
            {5, 764701200},
            {4, 780426000},
            {5, 796150800},
            {4, 811875600},
            {5, 828205200},
            {4, 846349200},
            {5, 859654800},
            {4, 877798800},
            {5, 891104400},
            {4, 909248400},
            {5, 922554000},
            {4, 941302800},
            {5, 954003600},
            {4, 972752400},
            {5, 985453200},
            {4, 1004202000},
            {5, 1017507600},
            {4, 1035651600},
            {5, 1048957200},
            {4, 1067101200},
            {5, 1080406800},
            {4, 1099155600},
            {5, 1111856400},
            {4, 1130605200},
            {5, 1143306000},
            {4, 1162054800},
            {5, 1174755600},
            {4, 1193504400},
            {5, 1206810000},
            {4, 1224954000},
            {5, 1238259600},
            {4, 1256403600},
            {5, 1269709200},
            {4, 1288458000},
            {8, 1301158800},
            {7, 1414252800},
            {3, 1459015200},
        }};
    }

    inline constexpr db::entry chita{"Asia/Chita",
                                     "MSK+06 - Zabaykalsky",
                                     "+5203+11328",
                                     detail::chita::country_codes,
                                     detail::chita::offsets,
                                     detail::chita::transitions};

    // clang-format on
}
