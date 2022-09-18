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

    namespace detail::guyana
    {
        inline constexpr array<country::code, 1> country_codes{
            "GY",
        };

        inline constexpr array<zone::offset, 5> offsets{{
            {"LMT", -13959, false},
            {"-04", -14400, false},
            {"-0345", -13500, false},
            {"-03", -10800, false},
            {"-04", -14400, false},
        }};

        inline constexpr array<zone::transition, 5> transitions{{
            {1, -1843589241},
            {2, -1730577600},
            {3, 176096700},
            {1, 701841600},
            {1, 2147483647},
        }};
    }

    inline constexpr db::entry guyana{"America/Guyana",
                                      "",
                                      "+0648-05810",
                                      detail::guyana::country_codes,
                                      detail::guyana::offsets,
                                      detail::guyana::transitions};

    // clang-format on
}
