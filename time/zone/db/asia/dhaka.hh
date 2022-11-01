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

    namespace detail::dhaka
    {
        inline constexpr array<country::code, 1> country_codes{
            "BD",
        };

        inline constexpr array<zone::offset, 6> offsets{{
            {"LMT", 21700, false},
            {"HMT", 21200, false},
            {"+0630", 23400, false},
            {"+0530", 19800, false},
            {"+06", 21600, false},
            {"+07", 25200, true},
        }};

        inline constexpr array<zone::transition, 7> transitions{{
            {1, -2524543300},
            {2, -891582800},
            {3, -872058600},
            {2, -862637400},
            {4, -576138600},
            {5, 1245430800},
            {4, 1262278800},
        }};
    }

    inline constexpr db::entry dhaka{"Asia/Dhaka",
                                     "",
                                     "+2343+09025",
                                     detail::dhaka::country_codes,
                                     detail::dhaka::offsets,
                                     detail::dhaka::transitions};

    // clang-format on
}
