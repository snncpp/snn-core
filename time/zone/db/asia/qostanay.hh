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

    namespace detail::qostanay
    {
        inline constexpr array<country::code, 1> country_codes{
            "KZ",
        };

        inline constexpr array<zone::offset, 12> offsets{{
            {"LMT", 15268, false},
            {"+04", 14400, false},
            {"+05", 18000, false},
            {"+06", 21600, true},
            {"+06", 21600, false},
            {"+05", 18000, false},
            {"+06", 21600, true},
            {"+05", 18000, true},
            {"+04", 14400, false},
            {"+06", 21600, false},
            {"+06", 21600, true},
            {"+05", 18000, false},
        }};

        inline constexpr array<zone::transition, 52> transitions{{
            {1, -1441167268},
            {2, -1247544000},
            {3, 354913200},
            {4, 370720800},
            {3, 386445600},
            {2, 402256800},
            {3, 417985200},
            {2, 433792800},
            {3, 449607600},
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
            {6, 922568400},
            {5, 941317200},
            {6, 954018000},
            {5, 972766800},
            {6, 985467600},
            {5, 1004216400},
            {6, 1017522000},
            {5, 1035666000},
            {6, 1048971600},
            {5, 1067115600},
            {6, 1080421200},
            {9, 1099170000},
            {2, 1709229600},
        }};
    }

    inline constexpr db::entry qostanay{"Asia/Qostanay",
                                        "Qostanay/Kostanay/Kustanay",
                                        "+5312+06337",
                                        detail::qostanay::country_codes,
                                        detail::qostanay::offsets,
                                        detail::qostanay::transitions};

    // clang-format on
}
