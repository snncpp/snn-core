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

    namespace detail::kiritimati
    {
        inline constexpr array<country::code, 1> country_codes{
            "KI",
        };

        inline constexpr array<zone::offset, 4> offsets{{
            {"LMT", -37760, false},
            {"-1040", -38400, false},
            {"-10", -36000, false},
            {"+14", 50400, false},
        }};

        inline constexpr array<zone::transition, 3> transitions{{
            {1, -2177415040},
            {2, 307622400},
            {3, 788868000},
        }};
    }

    inline constexpr db::entry kiritimati{"Pacific/Kiritimati",
                                          "Line Islands",
                                          "+0152-15720",
                                          detail::kiritimati::country_codes,
                                          detail::kiritimati::offsets,
                                          detail::kiritimati::transitions};

    // clang-format on
}
