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

namespace snn::time::zone::db::america
{
    // clang-format off

    namespace detail::swift_current
    {
        inline constexpr array<country::code, 1> country_codes{
            "CA",
        };

        inline constexpr array<zone::offset, 6> offsets{{
            {"LMT", -25880, false},
            {"MDT", -21600, true},
            {"MST", -25200, false},
            {"MWT", -21600, true},
            {"MPT", -21600, true},
            {"CST", -21600, false},
        }};

        inline constexpr array<zone::transition, 23> transitions{{
            {2, -2030201320},
            {1, -1632063600},
            {2, -1615132800},
            {3, -880210800},
            {4, -769395600},
            {2, -765388800},
            {1, -747241200},
            {2, -732729600},
            {1, -715791600},
            {2, -702489600},
            {1, -684342000},
            {2, -671040000},
            {1, -652892400},
            {2, -639590400},
            {1, -400086000},
            {2, -384364800},
            {1, -337186800},
            {2, -321465600},
            {1, -305737200},
            {2, -292435200},
            {1, -273682800},
            {2, -260985600},
            {5, 73472400},
        }};
    }

    inline constexpr db::entry swift_current{"America/Swift_Current",
                                             "CST - SK (midwest)",
                                             "+5017-10750",
                                             detail::swift_current::country_codes,
                                             detail::swift_current::offsets,
                                             detail::swift_current::transitions};

    // clang-format on
}
