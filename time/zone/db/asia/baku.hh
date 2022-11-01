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

    namespace detail::baku
    {
        inline constexpr array<country::code, 1> country_codes{
            "AZ",
        };

        inline constexpr array<zone::offset, 10> offsets{{
            {"LMT", 11964, false},
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

        inline constexpr array<zone::transition, 66> transitions{{
            {1, -1441163964},
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
            {4, 717548400},
            {8, 828234000},
            {9, 846378000},
            {2, 859680000},
            {3, 877824000},
            {2, 891129600},
            {3, 909273600},
            {2, 922579200},
            {3, 941328000},
            {2, 954028800},
            {3, 972777600},
            {2, 985478400},
            {3, 1004227200},
            {2, 1017532800},
            {3, 1035676800},
            {2, 1048982400},
            {3, 1067126400},
            {2, 1080432000},
            {3, 1099180800},
            {2, 1111881600},
            {3, 1130630400},
            {2, 1143331200},
            {3, 1162080000},
            {2, 1174780800},
            {3, 1193529600},
            {2, 1206835200},
            {3, 1224979200},
            {2, 1238284800},
            {3, 1256428800},
            {2, 1269734400},
            {3, 1288483200},
            {2, 1301184000},
            {3, 1319932800},
            {2, 1332633600},
            {3, 1351382400},
            {2, 1364688000},
            {3, 1382832000},
            {2, 1396137600},
            {3, 1414281600},
            {2, 1427587200},
            {3, 1445731200},
        }};
    }

    inline constexpr db::entry baku{"Asia/Baku",
                                    "",
                                    "+4023+04951",
                                    detail::baku::country_codes,
                                    detail::baku::offsets,
                                    detail::baku::transitions};

    // clang-format on
}
