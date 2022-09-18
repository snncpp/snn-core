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

    namespace detail::guayaquil
    {
        inline constexpr array<country::code, 1> country_codes{
            "EC",
        };

        inline constexpr array<zone::offset, 4> offsets{{
            {"LMT", -19160, false},
            {"QMT", -18840, false},
            {"-04", -14400, true},
            {"-05", -18000, false},
        }};

        inline constexpr array<zone::transition, 5> transitions{{
            {1, -2524502440},
            {3, -1230749160},
            {2, 722926800},
            {3, 728884800},
            {3, 2147483647},
        }};
    }

    inline constexpr db::entry guayaquil{"America/Guayaquil",
                                         "Ecuador (mainland)",
                                         "-0210-07950",
                                         detail::guayaquil::country_codes,
                                         detail::guayaquil::offsets,
                                         detail::guayaquil::transitions};

    // clang-format on
}
