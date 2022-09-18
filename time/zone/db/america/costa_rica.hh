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

    namespace detail::costa_rica
    {
        inline constexpr array<country::code, 1> country_codes{
            "CR",
        };

        inline constexpr array<zone::offset, 4> offsets{{
            {"LMT", -20173, false},
            {"SJMT", -20173, false},
            {"CDT", -18000, true},
            {"CST", -21600, false},
        }};

        inline constexpr array<zone::transition, 10> transitions{{
            {1, -2524501427},
            {3, -1545071027},
            {2, 288770400},
            {3, 297234000},
            {2, 320220000},
            {3, 328683600},
            {2, 664264800},
            {3, 678344400},
            {2, 695714400},
            {3, 700635600},
        }};
    }

    inline constexpr db::entry costa_rica{"America/Costa_Rica",
                                          "",
                                          "+0956-08405",
                                          detail::costa_rica::country_codes,
                                          detail::costa_rica::offsets,
                                          detail::costa_rica::transitions};

    // clang-format on
}
