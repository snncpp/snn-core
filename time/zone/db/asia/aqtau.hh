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

    namespace detail::aqtau
    {
        inline constexpr array<country::code, 1> country_codes{
            "KZ",
        };

        inline constexpr array<zone::offset, 10> offsets{{
            {"LMT", 12064, false},
            {"+04", 14400, false},
            {"+05", 18000, false},
            {"+06", 21600, false},
            {"+06", 21600, true},
            {"+05", 18000, false},
            {"+06", 21600, true},
            {"+05", 18000, true},
            {"+04", 14400, false},
            {"+05", 18000, false},
        }};

        inline constexpr array<zone::transition, 51> transitions{{
            {1, -1441164064},
            {2, -1247544000},
            {3, 370724400},
            {4, 386445600},
            {2, 402256800},
            {4, 417985200},
            {2, 433792800},
            {4, 449607600},
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
            {8, 780440400},
            {7, 796168800},
            {8, 811893600},
            {7, 828223200},
            {8, 846367200},
            {7, 859672800},
            {8, 877816800},
            {7, 891122400},
            {8, 909266400},
            {7, 922572000},
            {8, 941320800},
            {7, 954021600},
            {8, 972770400},
            {7, 985471200},
            {8, 1004220000},
            {7, 1017525600},
            {8, 1035669600},
            {7, 1048975200},
            {8, 1067119200},
            {7, 1080424800},
            {5, 1099173600},
            {5, 2147483647},
        }};
    }

    inline constexpr db::entry aqtau{"Asia/Aqtau",
                                     "Mangghystaū/Mankistau",
                                     "+4431+05016",
                                     detail::aqtau::country_codes,
                                     detail::aqtau::offsets,
                                     detail::aqtau::transitions};

    // clang-format on
}
