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

        inline constexpr array<zone::offset, 7> offsets{{
            {"LMT", -57368, false},
            {"LMT", 29032, false},
            {"PDT", 32400, true},
            {"PST", 28800, false},
            {"PST", 28800, false},
            {"JST", 32400, false},
            {"PST", 28800, false},
        }};

        inline constexpr array<zone::transition, 14> transitions{{
            {1, -3944621032},
            {4, -2219083200},
            {2, -1046678400},
            {3, -1040115600},
            {2, -885024000},
            {5, -880016400},
            {2, -783594000},
            {3, -760093200},
            {2, -496224000},
            {3, -491562000},
            {2, 228326400},
            {3, 243702000},
            {2, 643219200},
            {3, 649177200},
        }};
    }

    inline constexpr db::entry manila{"Asia/Manila",
                                      "",
                                      "+143512+1205804",
                                      detail::manila::country_codes,
                                      detail::manila::offsets,
                                      detail::manila::transitions};

    // clang-format on
}
