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

    namespace detail::taipei
    {
        inline constexpr array<country::code, 1> country_codes{
            "TW",
        };

        inline constexpr array<zone::offset, 5> offsets{{
            {"LMT", 29160, false},
            {"CST", 28800, false},
            {"JST", 32400, false},
            {"CDT", 32400, true},
            {"CST", 28800, false},
        }};

        inline constexpr array<zone::transition, 41> transitions{{
            {1, -2335248360},
            {2, -1017820800},
            {1, -766224000},
            {3, -745833600},
            {1, -733827600},
            {3, -716889600},
            {1, -699613200},
            {3, -683884800},
            {1, -670669200},
            {3, -652348800},
            {1, -639133200},
            {3, -620812800},
            {1, -607597200},
            {3, -589276800},
            {1, -576061200},
            {3, -562924800},
            {1, -541760400},
            {3, -528710400},
            {1, -510224400},
            {3, -497174400},
            {1, -478688400},
            {3, -465638400},
            {1, -449830800},
            {3, -434016000},
            {1, -418208400},
            {3, -402480000},
            {1, -386672400},
            {3, -370944000},
            {1, -355136400},
            {3, -339408000},
            {1, -323600400},
            {3, -302515200},
            {1, -291978000},
            {3, -270979200},
            {1, -260442000},
            {3, 133977600},
            {1, 149785200},
            {3, 165513600},
            {1, 181321200},
            {3, 299606400},
            {1, 307551600},
        }};
    }

    inline constexpr db::entry taipei{"Asia/Taipei",
                                      "",
                                      "+2503+12130",
                                      detail::taipei::country_codes,
                                      detail::taipei::offsets,
                                      detail::taipei::transitions};

    // clang-format on
}
