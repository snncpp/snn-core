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

    namespace detail::tehran
    {
        inline constexpr array<country::code, 1> country_codes{
            "IR",
        };

        inline constexpr array<zone::offset, 8> offsets{{
            {"LMT", 12344, false},
            {"TMT", 12344, false},
            {"+0430", 16200, true},
            {"+0330", 12600, false},
            {"+05", 18000, true},
            {"+04", 14400, false},
            {"+0430", 16200, true},
            {"+0330", 12600, false},
        }};

        inline constexpr array<zone::transition, 71> transitions{{
            {1, -1704165944},
            {3, -1090466744},
            {2, 227820600},
            {5, 246223800},
            {4, 259617600},
            {5, 271108800},
            {3, 283982400},
            {2, 296598600},
            {3, 306531000},
            {2, 322432200},
            {3, 338499000},
            {2, 673216200},
            {3, 685481400},
            {2, 701209800},
            {3, 717103800},
            {2, 732745800},
            {3, 748639800},
            {2, 764281800},
            {3, 780175800},
            {2, 795817800},
            {3, 811711800},
            {2, 827353800},
            {3, 843247800},
            {2, 858976200},
            {3, 874870200},
            {2, 890512200},
            {3, 906406200},
            {2, 922048200},
            {3, 937942200},
            {2, 953584200},
            {3, 969478200},
            {2, 985206600},
            {3, 1001100600},
            {2, 1016742600},
            {3, 1032636600},
            {2, 1048278600},
            {3, 1064172600},
            {2, 1079814600},
            {3, 1095708600},
            {2, 1111437000},
            {3, 1127331000},
            {2, 1206045000},
            {3, 1221939000},
            {2, 1237667400},
            {3, 1253561400},
            {2, 1269203400},
            {3, 1285097400},
            {2, 1300739400},
            {3, 1316633400},
            {2, 1332275400},
            {3, 1348169400},
            {2, 1363897800},
            {3, 1379791800},
            {2, 1395433800},
            {3, 1411327800},
            {2, 1426969800},
            {3, 1442863800},
            {2, 1458505800},
            {3, 1474399800},
            {2, 1490128200},
            {3, 1506022200},
            {2, 1521664200},
            {3, 1537558200},
            {2, 1553200200},
            {3, 1569094200},
            {2, 1584736200},
            {3, 1600630200},
            {2, 1616358600},
            {3, 1632252600},
            {2, 1647894600},
            {3, 1663788600},
        }};
    }

    inline constexpr db::entry tehran{"Asia/Tehran",
                                      "",
                                      "+3540+05126",
                                      detail::tehran::country_codes,
                                      detail::tehran::offsets,
                                      detail::tehran::transitions};

    // clang-format on
}
