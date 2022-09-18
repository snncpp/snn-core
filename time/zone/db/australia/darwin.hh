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

    namespace detail::darwin
    {
        inline constexpr array<country::code, 1> country_codes{
            "AU",
        };

        inline constexpr array<zone::offset, 5> offsets{{
            {"LMT", 31400, false},
            {"ACST", 32400, false},
            {"ACDT", 37800, true},
            {"ACST", 34200, false},
            {"ACST", 34200, false},
        }};

        inline constexpr array<zone::transition, 10> transitions{{
            {1, -2364108200},
            {4, -2230189200},
            {2, -1672558200},
            {3, -1665387000},
            {2, -883639800},
            {3, -876123000},
            {2, -860398200},
            {3, -844673400},
            {2, -828343800},
            {3, -813223800},
        }};
    }

    inline constexpr db::entry darwin{"Australia/Darwin",
                                      "Northern Territory",
                                      "-1228+13050",
                                      detail::darwin::country_codes,
                                      detail::darwin::offsets,
                                      detail::darwin::transitions};

    // clang-format on
}
