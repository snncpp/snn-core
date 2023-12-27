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

    namespace detail::makassar
    {
        inline constexpr array<country::code, 1> country_codes{
            "ID",
        };

        inline constexpr array<zone::offset, 5> offsets{{
            {"LMT", 28656, false},
            {"MMT", 28656, false},
            {"+08", 28800, false},
            {"+09", 32400, false},
            {"WITA", 28800, false},
        }};

        inline constexpr array<zone::transition, 4> transitions{{
            {1, -1577951856},
            {2, -1172908656},
            {3, -880272000},
            {4, -766054800},
        }};
    }

    inline constexpr db::entry makassar{"Asia/Makassar",
                                        "Borneo (east, south), Sulawesi/Celebes, Bali, Nusa Tengarra, Timor (west)",
                                        "-0507+11924",
                                        detail::makassar::country_codes,
                                        detail::makassar::offsets,
                                        detail::makassar::transitions};

    // clang-format on
}
