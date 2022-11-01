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

    namespace detail::samara
    {
        inline constexpr array<country::code, 1> country_codes{
            "RU",
        };

        inline constexpr array<zone::offset, 11> offsets{{
            {"LMT", 12020, false},
            {"+03", 10800, false},
            {"+04", 14400, false},
            {"+05", 18000, true},
            {"+04", 14400, false},
            {"+05", 18000, true},
            {"+04", 14400, true},
            {"+03", 10800, false},
            {"+03", 10800, true},
            {"+04", 14400, true},
            {"+04", 14400, false},
        }};

        inline constexpr array<zone::transition, 64> transitions{{
            {1, -1593820800},
            {2, -1247540400},
            {3, 354916800},
            {2, 370724400},
            {3, 386452800},
            {2, 402260400},
            {3, 417988800},
            {2, 433796400},
            {3, 449611200},
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
            {8, 670374000},
            {7, 686102400},
            {2, 687916800},
            {5, 701820000},
            {4, 717544800},
            {5, 733269600},
            {4, 748994400},
            {5, 764719200},
            {4, 780444000},
            {5, 796168800},
            {4, 811893600},
            {5, 828223200},
            {4, 846367200},
            {5, 859672800},
            {4, 877816800},
            {5, 891122400},
            {4, 909266400},
            {5, 922572000},
            {4, 941320800},
            {5, 954021600},
            {4, 972770400},
            {5, 985471200},
            {4, 1004220000},
            {5, 1017525600},
            {4, 1035669600},
            {5, 1048975200},
            {4, 1067119200},
            {5, 1080424800},
            {4, 1099173600},
            {5, 1111874400},
            {4, 1130623200},
            {5, 1143324000},
            {4, 1162072800},
            {5, 1174773600},
            {4, 1193522400},
            {5, 1206828000},
            {4, 1224972000},
            {5, 1238277600},
            {4, 1256421600},
            {6, 1269727200},
            {7, 1288479600},
            {4, 1301180400},
        }};
    }

    inline constexpr db::entry samara{"Europe/Samara",
                                      "MSK+01 - Samara, Udmurtia",
                                      "+5312+05009",
                                      detail::samara::country_codes,
                                      detail::samara::offsets,
                                      detail::samara::transitions};

    // clang-format on
}
