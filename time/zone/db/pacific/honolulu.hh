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

    namespace detail::honolulu
    {
        inline constexpr array<country::code, 1> country_codes{
            "US",
        };

        inline constexpr array<zone::offset, 6> offsets{{
            {"LMT", -37886, false},
            {"HST", -37800, false},
            {"HDT", -34200, true},
            {"HWT", -34200, true},
            {"HPT", -34200, true},
            {"HST", -36000, false},
        }};

        inline constexpr array<zone::transition, 7> transitions{{
            {1, -2334101314},
            {2, -1157283000},
            {1, -1155436200},
            {3, -880198200},
            {4, -769395600},
            {1, -765376200},
            {5, -712150200},
        }};
    }

    inline constexpr db::entry honolulu{"Pacific/Honolulu",
                                        "Hawaii",
                                        "+211825-1575130",
                                        detail::honolulu::country_codes,
                                        detail::honolulu::offsets,
                                        detail::honolulu::transitions};

    // clang-format on
}
