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

namespace snn::time::zone::db::pacific
{
    // clang-format off

    namespace detail::efate
    {
        inline constexpr array<country::code, 1> country_codes{
            "VU",
        };

        inline constexpr array<zone::offset, 5> offsets{{
            {"LMT", 40396, false},
            {"+12", 43200, true},
            {"+11", 39600, false},
            {"+12", 43200, true},
            {"+11", 39600, false},
        }};

        inline constexpr array<zone::transition, 23> transitions{{
            {4, -1829387596},
            {1, 125409600},
            {2, 133876800},
            {3, 433256400},
            {4, 448977600},
            {3, 464706000},
            {4, 480427200},
            {3, 496760400},
            {4, 511876800},
            {3, 528210000},
            {4, 543931200},
            {3, 559659600},
            {4, 575380800},
            {3, 591109200},
            {4, 606830400},
            {3, 622558800},
            {4, 638280000},
            {3, 654008400},
            {4, 669729600},
            {3, 686062800},
            {4, 696340800},
            {3, 719931600},
            {4, 727790400},
        }};
    }

    inline constexpr db::entry efate{"Pacific/Efate",
                                     "",
                                     "-1740+16825",
                                     detail::efate::country_codes,
                                     detail::efate::offsets,
                                     detail::efate::transitions};

    // clang-format on
}
