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

    namespace detail::panama
    {
        inline constexpr array<country::code, 3> country_codes{
            "PA",
            "CA",
            "KY",
        };

        inline constexpr array<zone::offset, 3> offsets{{
            {"LMT", -19088, false},
            {"CMT", -19176, false},
            {"EST", -18000, false},
        }};

        inline constexpr array<zone::transition, 2> transitions{{
            {1, -2524502512},
            {2, -1946918424},
        }};
    }

    inline constexpr db::entry panama{"America/Panama",
                                      "EST - ON (Atikokan), NU (Coral H)",
                                      "+0858-07932",
                                      detail::panama::country_codes,
                                      detail::panama::offsets,
                                      detail::panama::transitions};

    // clang-format on
}
