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

    namespace detail::shanghai
    {
        inline constexpr array<country::code, 1> country_codes{
            "CN",
        };

        inline constexpr array<zone::offset, 3> offsets{{
            {"LMT", 29143, false},
            {"CDT", 32400, true},
            {"CST", 28800, false},
        }};

        inline constexpr array<zone::transition, 29> transitions{{
            {2, -2177481943},
            {1, -1600675200},
            {2, -1585904400},
            {1, -933667200},
            {2, -922093200},
            {1, -908870400},
            {2, -888829200},
            {1, -881049600},
            {2, -767869200},
            {1, -745833600},
            {2, -733827600},
            {1, -716889600},
            {2, -699613200},
            {1, -683884800},
            {2, -670669200},
            {1, -652348800},
            {2, -650019600},
            {1, 515527200},
            {2, 527014800},
            {1, 545162400},
            {2, 558464400},
            {1, 577216800},
            {2, 589914000},
            {1, 608666400},
            {2, 621968400},
            {1, 640116000},
            {2, 653418000},
            {1, 671565600},
            {2, 684867600},
        }};
    }

    inline constexpr db::entry shanghai{"Asia/Shanghai",
                                        "Beijing Time",
                                        "+3114+12128",
                                        detail::shanghai::country_codes,
                                        detail::shanghai::offsets,
                                        detail::shanghai::transitions};

    // clang-format on
}
