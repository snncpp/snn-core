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

    namespace detail::kosrae
    {
        inline constexpr array<country::code, 1> country_codes{
            "FM",
        };

        inline constexpr array<zone::offset, 7> offsets{{
            {"LMT", -47284, false},
            {"LMT", 39116, false},
            {"+11", 39600, false},
            {"+09", 32400, false},
            {"+10", 36000, false},
            {"+12", 43200, false},
            {"+11", 39600, false},
        }};

        inline constexpr array<zone::transition, 9> transitions{{
            {1, -3944631116},
            {2, -2177491916},
            {3, -1743678000},
            {2, -1606813200},
            {4, -1041418800},
            {3, -907408800},
            {2, -770634000},
            {5, -7988400},
            {2, 915105600},
        }};
    }

    inline constexpr db::entry kosrae{"Pacific/Kosrae",
                                      "Kosrae",
                                      "+0519+16259",
                                      detail::kosrae::country_codes,
                                      detail::kosrae::offsets,
                                      detail::kosrae::transitions};

    // clang-format on
}
