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

    namespace detail::tripoli
    {
        inline constexpr array<country::code, 1> country_codes{
            "LY",
        };

        inline constexpr array<zone::offset, 4> offsets{{
            {"LMT", 3164, false},
            {"CEST", 7200, true},
            {"CET", 3600, false},
            {"EET", 7200, false},
        }};

        inline constexpr array<zone::transition, 32> transitions{{
            {2, -1577926364},
            {1, -574902000},
            {2, -568087200},
            {1, -512175600},
            {2, -504928800},
            {1, -449888400},
            {2, -441856800},
            {3, -347158800},
            {2, 378684000},
            {1, 386463600},
            {2, 402271200},
            {1, 417999600},
            {2, 433807200},
            {1, 449622000},
            {2, 465429600},
            {1, 481590000},
            {2, 496965600},
            {1, 512953200},
            {2, 528674400},
            {1, 544230000},
            {2, 560037600},
            {1, 575852400},
            {2, 591660000},
            {1, 607388400},
            {2, 623196000},
            {3, 641775600},
            {2, 844034400},
            {1, 860108400},
            {3, 875916000},
            {2, 1352505600},
            {1, 1364515200},
            {3, 1382659200},
        }};
    }

    inline constexpr db::entry tripoli{"Africa/Tripoli",
                                       "",
                                       "+3254+01311",
                                       detail::tripoli::country_codes,
                                       detail::tripoli::offsets,
                                       detail::tripoli::transitions};

    // clang-format on
}
