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

namespace snn::time::zone::db::africa
{
    // clang-format off

    namespace detail::juba
    {
        inline constexpr array<country::code, 1> country_codes{
            "SS",
        };

        inline constexpr array<zone::offset, 5> offsets{{
            {"LMT", 7588, false},
            {"CAST", 10800, true},
            {"CAT", 7200, false},
            {"EAT", 10800, false},
            {"CAT", 7200, false},
        }};

        inline constexpr array<zone::transition, 35> transitions{{
            {2, -1230775588},
            {1, 10360800},
            {2, 24786000},
            {1, 41810400},
            {2, 56322000},
            {1, 73432800},
            {2, 87944400},
            {1, 104882400},
            {2, 119480400},
            {1, 136332000},
            {2, 151016400},
            {1, 167781600},
            {2, 182552400},
            {1, 199231200},
            {2, 214174800},
            {1, 230680800},
            {2, 245710800},
            {1, 262735200},
            {2, 277246800},
            {1, 294184800},
            {2, 308782800},
            {1, 325634400},
            {2, 340405200},
            {1, 357084000},
            {2, 371941200},
            {1, 388533600},
            {2, 403477200},
            {1, 419983200},
            {2, 435013200},
            {1, 452037600},
            {2, 466635600},
            {1, 483487200},
            {2, 498171600},
            {3, 947930400},
            {2, 1612126800},
        }};
    }

    inline constexpr db::entry juba{"Africa/Juba",
                                    "",
                                    "+0451+03137",
                                    detail::juba::country_codes,
                                    detail::juba::offsets,
                                    detail::juba::transitions};

    // clang-format on
}
