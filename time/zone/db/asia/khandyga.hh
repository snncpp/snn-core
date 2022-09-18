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

    namespace detail::khandyga
    {
        inline constexpr array<country::code, 1> country_codes{
            "RU",
        };

        inline constexpr array<zone::offset, 13> offsets{{
            {"LMT", 32533, false},
            {"+08", 28800, false},
            {"+10", 36000, true},
            {"+09", 32400, false},
            {"+09", 32400, false},
            {"+10", 36000, true},
            {"+09", 32400, true},
            {"+08", 28800, false},
            {"+11", 39600, true},
            {"+10", 36000, false},
            {"+10", 36000, false},
            {"+11", 39600, false},
            {"+09", 32400, false},
        }};

        inline constexpr array<zone::transition, 68> transitions{{
            {1, -1579424533},
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
            {10, 1072882800},
            {8, 1080403200},
            {9, 1099152000},
            {8, 1111852800},
            {9, 1130601600},
            {8, 1143302400},
            {9, 1162051200},
            {8, 1174752000},
            {9, 1193500800},
            {8, 1206806400},
            {9, 1224950400},
            {8, 1238256000},
            {9, 1256400000},
            {8, 1269705600},
            {9, 1288454400},
            {11, 1301155200},
            {9, 1315832400},
            {4, 1414252800},
            {4, 2147483647},
        }};
    }

    inline constexpr db::entry khandyga{"Asia/Khandyga",
                                        "MSK+06 - Tomponsky, Ust-Maysky",
                                        "+623923+1353314",
                                        detail::khandyga::country_codes,
                                        detail::khandyga::offsets,
                                        detail::khandyga::transitions};

    // clang-format on
}
