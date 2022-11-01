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

    namespace detail::atyrau
    {
        inline constexpr array<country::code, 1> country_codes{
            "KZ",
        };

        inline constexpr array<zone::offset, 10> offsets{{
            {"LMT", 12464, false},
            {"+03", 10800, false},
            {"+05", 18000, false},
            {"+06", 21600, false},
            {"+06", 21600, true},
            {"+05", 18000, false},
            {"+06", 21600, true},
            {"+05", 18000, true},
            {"+04", 14400, false},
            {"+05", 18000, false},
        }};

        inline constexpr array<zone::transition, 50> transitions{{
            {1, -1441164464},
            {2, -1247540400},
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
            {5, 780440400},
            {6, 796165200},
            {5, 811890000},
            {6, 828219600},
            {5, 846363600},
            {6, 859669200},
            {5, 877813200},
            {6, 891118800},
            {5, 909262800},
            {7, 922568400},
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
        }};
    }

    inline constexpr db::entry atyrau{"Asia/Atyrau",
                                      "Atyraū/Atirau/Gur'yev",
                                      "+4707+05156",
                                      detail::atyrau::country_codes,
                                      detail::atyrau::offsets,
                                      detail::atyrau::transitions};

    // clang-format on
}
