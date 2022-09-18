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

    namespace detail::johannesburg
    {
        inline constexpr array<country::code, 3> country_codes{
            "ZA",
            "LS",
            "SZ",
        };

        inline constexpr array<zone::offset, 4> offsets{{
            {"LMT", 6720, false},
            {"SAST", 5400, false},
            {"SAST", 10800, true},
            {"SAST", 7200, false},
        }};

        inline constexpr array<zone::transition, 6> transitions{{
            {1, -2458173120},
            {3, -2109288600},
            {2, -860976000},
            {3, -845254800},
            {2, -829526400},
            {3, -813805200},
        }};
    }

    inline constexpr db::entry johannesburg{"Africa/Johannesburg",
                                            "",
                                            "-2615+02800",
                                            detail::johannesburg::country_codes,
                                            detail::johannesburg::offsets,
                                            detail::johannesburg::transitions};

    // clang-format on
}
