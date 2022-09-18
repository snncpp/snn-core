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

    namespace detail::bogota
    {
        inline constexpr array<country::code, 1> country_codes{
            "CO",
        };

        inline constexpr array<zone::offset, 4> offsets{{
            {"LMT", -17776, false},
            {"BMT", -17776, false},
            {"-04", -14400, true},
            {"-05", -18000, false},
        }};

        inline constexpr array<zone::transition, 5> transitions{{
            {1, -2707671824},
            {3, -1739041424},
            {2, 704869200},
            {3, 733896000},
            {3, 2147483647},
        }};
    }

    inline constexpr db::entry bogota{"America/Bogota",
                                      "",
                                      "+0436-07405",
                                      detail::bogota::country_codes,
                                      detail::bogota::offsets,
                                      detail::bogota::transitions};

    // clang-format on
}
