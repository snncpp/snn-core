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

    namespace detail::colombo
    {
        inline constexpr array<country::code, 1> country_codes{
            "LK",
        };

        inline constexpr array<zone::offset, 8> offsets{{
            {"LMT", 19164, false},
            {"MMT", 19172, false},
            {"+0530", 19800, false},
            {"+06", 21600, true},
            {"+0630", 23400, true},
            {"+0630", 23400, false},
            {"+06", 21600, false},
            {"+0530", 19800, false},
        }};

        inline constexpr array<zone::transition, 9> transitions{{
            {1, -2840159964},
            {2, -2019705572},
            {3, -883287000},
            {4, -862639200},
            {2, -764051400},
            {5, 832962600},
            {6, 846266400},
            {2, 1145039400},
            {2, 2147483647},
        }};
    }

    inline constexpr db::entry colombo{"Asia/Colombo",
                                       "",
                                       "+0656+07951",
                                       detail::colombo::country_codes,
                                       detail::colombo::offsets,
                                       detail::colombo::transitions};

    // clang-format on
}
