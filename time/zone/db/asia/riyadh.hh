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

    namespace detail::riyadh
    {
        inline constexpr array<country::code, 4> country_codes{
            "SA",
            "AQ",
            "KW",
            "YE",
        };

        inline constexpr array<zone::offset, 2> offsets{{
            {"LMT", 11212, false},
            {"+03", 10800, false},
        }};

        inline constexpr array<zone::transition, 1> transitions{{
            {1, -719636812},
        }};
    }

    inline constexpr db::entry riyadh{"Asia/Riyadh",
                                      "Arabia, Syowa",
                                      "+2438+04643",
                                      detail::riyadh::country_codes,
                                      detail::riyadh::offsets,
                                      detail::riyadh::transitions};

    // clang-format on
}
