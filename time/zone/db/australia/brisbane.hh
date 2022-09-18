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

namespace snn::time::zone::db::australia
{
    // clang-format off

    namespace detail::brisbane
    {
        inline constexpr array<country::code, 1> country_codes{
            "AU",
        };

        inline constexpr array<zone::offset, 4> offsets{{
            {"LMT", 36728, false},
            {"AEDT", 39600, true},
            {"AEST", 36000, false},
            {"AEST", 36000, false},
        }};

        inline constexpr array<zone::transition, 17> transitions{{
            {3, -2366791928},
            {1, -1672560000},
            {2, -1665388800},
            {1, -883641600},
            {2, -876124800},
            {1, -860400000},
            {2, -844675200},
            {1, -828345600},
            {2, -813225600},
            {1, 57686400},
            {2, 67968000},
            {1, 625593600},
            {2, 636480000},
            {1, 657043200},
            {2, 667929600},
            {1, 688492800},
            {2, 699379200},
        }};
    }

    inline constexpr db::entry brisbane{"Australia/Brisbane",
                                        "Queensland (most areas)",
                                        "-2728+15302",
                                        detail::brisbane::country_codes,
                                        detail::brisbane::offsets,
                                        detail::brisbane::transitions};

    // clang-format on
}
