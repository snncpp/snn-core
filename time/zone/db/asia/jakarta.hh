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

    namespace detail::jakarta
    {
        inline constexpr array<country::code, 1> country_codes{
            "ID",
        };

        inline constexpr array<zone::offset, 7> offsets{{
            {"LMT", 25632, false},
            {"BMT", 25632, false},
            {"+0720", 26400, false},
            {"+0730", 27000, false},
            {"+09", 32400, false},
            {"+08", 28800, false},
            {"WIB", 25200, false},
        }};

        inline constexpr array<zone::transition, 8> transitions{{
            {1, -3231299232},
            {2, -1451719200},
            {3, -1172906400},
            {4, -876641400},
            {3, -766054800},
            {5, -683883000},
            {3, -620812800},
            {6, -189415800},
        }};
    }

    inline constexpr db::entry jakarta{"Asia/Jakarta",
                                       "Java, Sumatra",
                                       "-0610+10648",
                                       detail::jakarta::country_codes,
                                       detail::jakarta::offsets,
                                       detail::jakarta::transitions};

    // clang-format on
}
