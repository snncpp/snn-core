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

namespace snn::time::zone::db::australia
{
    // clang-format off

    namespace detail::perth
    {
        inline constexpr array<country::code, 1> country_codes{
            "AU",
        };

        inline constexpr array<zone::offset, 4> offsets{{
            {"LMT", 27804, false},
            {"AWDT", 32400, true},
            {"AWST", 28800, false},
            {"AWST", 28800, false},
        }};

        inline constexpr array<zone::transition, 19> transitions{{
            {3, -2337925404},
            {1, -1672552800},
            {2, -1665381600},
            {1, -883634400},
            {2, -876117600},
            {1, -860392800},
            {2, -844668000},
            {1, 152042400},
            {2, 162928800},
            {1, 436298400},
            {2, 447184800},
            {1, 690314400},
            {2, 699386400},
            {1, 1165082400},
            {2, 1174759200},
            {1, 1193508000},
            {2, 1206813600},
            {1, 1224957600},
            {2, 1238263200},
        }};
    }

    inline constexpr db::entry perth{"Australia/Perth",
                                     "Western Australia (most areas)",
                                     "-3157+11551",
                                     detail::perth::country_codes,
                                     detail::perth::offsets,
                                     detail::perth::transitions};

    // clang-format on
}
