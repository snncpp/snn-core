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

    namespace detail::ulaanbaatar
    {
        inline constexpr array<country::code, 1> country_codes{
            "MN",
        };

        inline constexpr array<zone::offset, 4> offsets{{
            {"LMT", 25652, false},
            {"+07", 25200, false},
            {"+09", 32400, true},
            {"+08", 28800, false},
        }};

        inline constexpr array<zone::transition, 50> transitions{{
            {1, -2032931252},
            {3, 252435600},
            {2, 417974400},
            {3, 433782000},
            {2, 449596800},
            {3, 465318000},
            {2, 481046400},
            {3, 496767600},
            {2, 512496000},
            {3, 528217200},
            {2, 543945600},
            {3, 559666800},
            {2, 575395200},
            {3, 591116400},
            {2, 606844800},
            {3, 622566000},
            {2, 638294400},
            {3, 654620400},
            {2, 670348800},
            {3, 686070000},
            {2, 701798400},
            {3, 717519600},
            {2, 733248000},
            {3, 748969200},
            {2, 764697600},
            {3, 780418800},
            {2, 796147200},
            {3, 811868400},
            {2, 828201600},
            {3, 843922800},
            {2, 859651200},
            {3, 875372400},
            {2, 891100800},
            {3, 906822000},
            {2, 988394400},
            {3, 1001696400},
            {2, 1017424800},
            {3, 1033146000},
            {2, 1048874400},
            {3, 1064595600},
            {2, 1080324000},
            {3, 1096045200},
            {2, 1111773600},
            {3, 1127494800},
            {2, 1143223200},
            {3, 1159549200},
            {2, 1427479200},
            {3, 1443193200},
            {2, 1458928800},
            {3, 1474642800},
        }};
    }

    inline constexpr db::entry ulaanbaatar{"Asia/Ulaanbaatar",
                                           "most of Mongolia",
                                           "+4755+10653",
                                           detail::ulaanbaatar::country_codes,
                                           detail::ulaanbaatar::offsets,
                                           detail::ulaanbaatar::transitions};

    // clang-format on
}
