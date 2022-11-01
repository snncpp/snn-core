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

    namespace detail::dushanbe
    {
        inline constexpr array<country::code, 1> country_codes{
            "TJ",
        };

        inline constexpr array<zone::offset, 8> offsets{{
            {"LMT", 16512, false},
            {"+05", 18000, false},
            {"+07", 25200, true},
            {"+06", 21600, false},
            {"+06", 21600, false},
            {"+07", 25200, true},
            {"+06", 21600, true},
            {"+05", 18000, false},
        }};

        inline constexpr array<zone::transition, 24> transitions{{
            {1, -1441168512},
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
            {7, 684363600},
        }};
    }

    inline constexpr db::entry dushanbe{"Asia/Dushanbe",
                                        "",
                                        "+3835+06848",
                                        detail::dushanbe::country_codes,
                                        detail::dushanbe::offsets,
                                        detail::dushanbe::transitions};

    // clang-format on
}
