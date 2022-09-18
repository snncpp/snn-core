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

    namespace detail::manila
    {
        inline constexpr array<country::code, 1> country_codes{
            "PH",
        };

        inline constexpr array<zone::offset, 6> offsets{{
            {"LMT", -57360, false},
            {"LMT", 29040, false},
            {"PDT", 32400, true},
            {"PST", 28800, false},
            {"JST", 32400, false},
            {"PST", 28800, false},
        }};

        inline constexpr array<zone::transition, 10> transitions{{
            {1, -3944621040},
            {3, -2229321840},
            {2, -1046678400},
            {3, -1038733200},
            {4, -873273600},
            {3, -794221200},
            {2, -496224000},
            {3, -489315600},
            {2, 259344000},
            {3, 275151600},
        }};
    }

    inline constexpr db::entry manila{"Asia/Manila",
                                      "",
                                      "+1435+12100",
                                      detail::manila::country_codes,
                                      detail::manila::offsets,
                                      detail::manila::transitions};

    // clang-format on
}
