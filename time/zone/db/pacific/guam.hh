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

    namespace detail::guam
    {
        inline constexpr array<country::code, 2> country_codes{
            "GU",
            "MP",
        };

        inline constexpr array<zone::offset, 6> offsets{{
            {"LMT", -51660, false},
            {"LMT", 34740, false},
            {"GST", 36000, false},
            {"+09", 32400, false},
            {"GDT", 39600, true},
            {"ChST", 36000, false},
        }};

        inline constexpr array<zone::transition, 21> transitions{{
            {1, -3944626740},
            {2, -2177487540},
            {3, -885549600},
            {2, -802256400},
            {4, -331891200},
            {2, -281610000},
            {4, -73728000},
            {2, -29415540},
            {4, -16704000},
            {2, -10659600},
            {4, 9907200},
            {2, 21394800},
            {4, 41356800},
            {2, 52844400},
            {4, 124819200},
            {2, 130863600},
            {4, 201888000},
            {2, 209487660},
            {4, 230659200},
            {2, 241542000},
            {5, 977493600},
        }};
    }

    inline constexpr db::entry guam{"Pacific/Guam",
                                    "",
                                    "+1328+14445",
                                    detail::guam::country_codes,
                                    detail::guam::offsets,
                                    detail::guam::transitions};

    // clang-format on
}
