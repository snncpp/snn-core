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

    namespace detail::bishkek
    {
        inline constexpr array<country::code, 1> country_codes{
            "KG",
        };

        inline constexpr array<zone::offset, 8> offsets{{
            {"LMT", 17904, false},
            {"+05", 18000, false},
            {"+07", 25200, true},
            {"+06", 21600, false},
            {"+06", 21600, false},
            {"+07", 25200, true},
            {"+06", 21600, true},
            {"+06", 21600, true},
        }};

        inline constexpr array<zone::transition, 52> transitions{{
            {1, -1441169904},
            {3, -1247547600},
            {2, 354909600},
            {3, 370717200},
            {2, 386445600},
            {3, 402253200},
            {2, 417981600},
            {3, 433789200},
            {2, 449604000},
            {4, 465336000},
            {5, 481060800},
            {4, 496785600},
            {5, 512510400},
            {4, 528235200},
            {5, 543960000},
            {4, 559684800},
            {5, 575409600},
            {4, 591134400},
            {5, 606859200},
            {4, 622584000},
            {5, 638308800},
            {4, 654638400},
            {6, 670363200},
            {1, 683582400},
            {6, 703018800},
            {1, 717530400},
            {6, 734468400},
            {1, 748980000},
            {6, 765918000},
            {1, 780429600},
            {6, 797367600},
            {1, 811879200},
            {6, 828817200},
            {1, 843933600},
            {7, 859671000},
            {1, 877811400},
            {7, 891120600},
            {1, 909261000},
            {7, 922570200},
            {1, 941315400},
            {7, 954019800},
            {1, 972765000},
            {7, 985469400},
            {1, 1004214600},
            {7, 1017523800},
            {1, 1035664200},
            {7, 1048973400},
            {1, 1067113800},
            {7, 1080423000},
            {1, 1099168200},
            {7, 1111872600},
            {3, 1123783200},
        }};
    }

    inline constexpr db::entry bishkek{"Asia/Bishkek",
                                       "",
                                       "+4254+07436",
                                       detail::bishkek::country_codes,
                                       detail::bishkek::offsets,
                                       detail::bishkek::transitions};

    // clang-format on
}
