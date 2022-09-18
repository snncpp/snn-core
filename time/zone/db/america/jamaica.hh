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

    namespace detail::jamaica
    {
        inline constexpr array<country::code, 1> country_codes{
            "JM",
        };

        inline constexpr array<zone::offset, 4> offsets{{
            {"LMT", -18430, false},
            {"KMT", -18430, false},
            {"EST", -18000, false},
            {"EDT", -14400, true},
        }};

        inline constexpr array<zone::transition, 22> transitions{{
            {1, -2524503170},
            {2, -1827687170},
            {3, 126687600},
            {2, 152085600},
            {3, 162370800},
            {2, 183535200},
            {3, 199263600},
            {2, 215589600},
            {3, 230713200},
            {2, 247039200},
            {3, 262767600},
            {2, 278488800},
            {3, 294217200},
            {2, 309938400},
            {3, 325666800},
            {2, 341388000},
            {3, 357116400},
            {2, 372837600},
            {3, 388566000},
            {2, 404892000},
            {3, 420015600},
            {2, 436341600},
        }};
    }

    inline constexpr db::entry jamaica{"America/Jamaica",
                                       "",
                                       "+175805-0764736",
                                       detail::jamaica::country_codes,
                                       detail::jamaica::offsets,
                                       detail::jamaica::transitions};

    // clang-format on
}
