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

namespace snn::time::zone::db::europe
{
    // clang-format off

    namespace detail::kirov
    {
        inline constexpr array<country::code, 1> country_codes{
            "RU",
        };

        inline constexpr array<zone::offset, 10> offsets{{
            {"LMT", 11928, false},
            {"+03", 10800, false},
            {"+05", 18000, true},
            {"+04", 14400, false},
            {"+04", 14400, false},
            {"+05", 18000, true},
            {"MSD", 14400, true},
            {"MSK", 10800, false},
            {"MSK", 14400, false},
            {"MSK", 10800, false},
        }};

        inline constexpr array<zone::transition, 63> transitions{{
            {1, -1593820800},
            {3, -1247540400},
            {2, 354916800},
            {3, 370724400},
            {2, 386452800},
            {3, 402260400},
            {2, 417988800},
            {3, 433796400},
            {2, 449611200},
            {4, 465343200},
            {5, 481068000},
            {4, 496792800},
            {5, 512517600},
            {4, 528242400},
            {5, 543967200},
            {4, 559692000},
            {5, 575416800},
            {4, 591141600},
            {6, 606866400},
            {7, 622594800},
            {6, 638319600},
            {7, 654649200},
            {4, 670374000},
            {6, 701820000},
            {7, 717548400},
            {6, 733273200},
            {7, 748998000},
            {6, 764722800},
            {7, 780447600},
            {6, 796172400},
            {7, 811897200},
            {6, 828226800},
            {7, 846370800},
            {6, 859676400},
            {7, 877820400},
            {6, 891126000},
            {7, 909270000},
            {6, 922575600},
            {7, 941324400},
            {6, 954025200},
            {7, 972774000},
            {6, 985474800},
            {7, 1004223600},
            {6, 1017529200},
            {7, 1035673200},
            {6, 1048978800},
            {7, 1067122800},
            {6, 1080428400},
            {7, 1099177200},
            {6, 1111878000},
            {7, 1130626800},
            {6, 1143327600},
            {7, 1162076400},
            {6, 1174777200},
            {7, 1193526000},
            {6, 1206831600},
            {7, 1224975600},
            {6, 1238281200},
            {7, 1256425200},
            {6, 1269730800},
            {7, 1288479600},
            {8, 1301180400},
            {7, 1414274400},
        }};
    }

    inline constexpr db::entry kirov{"Europe/Kirov",
                                     "MSK+00 - Kirov",
                                     "+5836+04939",
                                     detail::kirov::country_codes,
                                     detail::kirov::offsets,
                                     detail::kirov::transitions};

    // clang-format on
}
