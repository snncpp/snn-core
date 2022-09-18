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

    namespace detail::tegucigalpa
    {
        inline constexpr array<country::code, 1> country_codes{
            "HN",
        };

        inline constexpr array<zone::offset, 3> offsets{{
            {"LMT", -20932, false},
            {"CDT", -18000, true},
            {"CST", -21600, false},
        }};

        inline constexpr array<zone::transition, 7> transitions{{
            {2, -1538503868},
            {1, 547020000},
            {2, 559717200},
            {1, 578469600},
            {2, 591166800},
            {1, 1146981600},
            {2, 1154926800},
        }};
    }

    inline constexpr db::entry tegucigalpa{"America/Tegucigalpa",
                                           "",
                                           "+1406-08713",
                                           detail::tegucigalpa::country_codes,
                                           detail::tegucigalpa::offsets,
                                           detail::tegucigalpa::transitions};

    // clang-format on
}
