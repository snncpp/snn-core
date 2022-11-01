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

    namespace detail::kuching
    {
        inline constexpr array<country::code, 2> country_codes{
            "MY",
            "BN",
        };

        inline constexpr array<zone::offset, 6> offsets{{
            {"LMT", 26480, false},
            {"+0730", 27000, false},
            {"+0820", 30000, true},
            {"+08", 28800, false},
            {"+09", 32400, false},
            {"+08", 28800, false},
        }};

        inline constexpr array<zone::transition, 18> transitions{{
            {1, -1383463280},
            {3, -1167636600},
            {2, -1082448000},
            {3, -1074586800},
            {2, -1050825600},
            {3, -1042964400},
            {2, -1019289600},
            {3, -1011428400},
            {2, -987753600},
            {3, -979892400},
            {2, -956217600},
            {3, -948356400},
            {2, -924595200},
            {3, -916734000},
            {2, -893059200},
            {3, -885198000},
            {4, -879667200},
            {3, -767005200},
        }};
    }

    inline constexpr db::entry kuching{"Asia/Kuching",
                                       "Sabah, Sarawak, Brunei",
                                       "+0133+11020",
                                       detail::kuching::country_codes,
                                       detail::kuching::offsets,
                                       detail::kuching::transitions};

    // clang-format on
}
