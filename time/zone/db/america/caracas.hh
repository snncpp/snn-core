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

    namespace detail::caracas
    {
        inline constexpr array<country::code, 1> country_codes{
            "VE",
        };

        inline constexpr array<zone::offset, 4> offsets{{
            {"LMT", -16064, false},
            {"CMT", -16060, false},
            {"-0430", -16200, false},
            {"-04", -14400, false},
        }};

        inline constexpr array<zone::transition, 6> transitions{{
            {1, -2524505536},
            {2, -1826739140},
            {3, -157750200},
            {2, 1197183600},
            {3, 1462086000},
            {3, 2147483647},
        }};
    }

    inline constexpr db::entry caracas{"America/Caracas",
                                       "",
                                       "+1030-06656",
                                       detail::caracas::country_codes,
                                       detail::caracas::offsets,
                                       detail::caracas::transitions};

    // clang-format on
}
