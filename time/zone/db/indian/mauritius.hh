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

namespace snn::time::zone::db::indian
{
    // clang-format off

    namespace detail::mauritius
    {
        inline constexpr array<country::code, 1> country_codes{
            "MU",
        };

        inline constexpr array<zone::offset, 3> offsets{{
            {"LMT", 13800, false},
            {"+05", 18000, true},
            {"+04", 14400, false},
        }};

        inline constexpr array<zone::transition, 5> transitions{{
            {2, -1988164200},
            {1, 403041600},
            {2, 417034800},
            {1, 1224972000},
            {2, 1238274000},
        }};
    }

    inline constexpr db::entry mauritius{"Indian/Mauritius",
                                         "",
                                         "-2010+05730",
                                         detail::mauritius::country_codes,
                                         detail::mauritius::offsets,
                                         detail::mauritius::transitions};

    // clang-format on
}
