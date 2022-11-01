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

    namespace detail::yerevan
    {
        inline constexpr array<country::code, 1> country_codes{
            "AM",
        };

        inline constexpr array<zone::offset, 10> offsets{{
            {"LMT", 10680, false},
            {"+03", 10800, false},
            {"+05", 18000, true},
            {"+04", 14400, false},
            {"+04", 14400, false},
            {"+05", 18000, true},
            {"+04", 14400, true},
            {"+03", 10800, false},
            {"+05", 18000, true},
            {"+04", 14400, false},
        }};

        inline constexpr array<zone::transition, 62> transitions{{
            {1, -1441162680},
            {3, -405140400},
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
            {5, 606866400},
            {4, 622591200},
            {5, 638316000},
            {4, 654645600},
            {6, 670370400},
            {7, 686098800},
            {6, 701823600},
            {7, 717548400},
            {6, 733273200},
            {7, 748998000},
            {6, 764722800},
            {7, 780447600},
            {6, 796172400},
            {4, 811897200},
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
            {5, 1269727200},
            {4, 1288476000},
            {5, 1301176800},
            {4, 1319925600},
        }};
    }

    inline constexpr db::entry yerevan{"Asia/Yerevan",
                                       "",
                                       "+4011+04430",
                                       detail::yerevan::country_codes,
                                       detail::yerevan::offsets,
                                       detail::yerevan::transitions};

    // clang-format on
}
