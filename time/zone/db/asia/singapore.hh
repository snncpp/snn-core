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

    namespace detail::singapore
    {
        inline constexpr array<country::code, 2> country_codes{
            "SG",
            "MY",
        };

        inline constexpr array<zone::offset, 8> offsets{{
            {"LMT", 24925, false},
            {"SMT", 24925, false},
            {"+07", 25200, false},
            {"+0720", 26400, true},
            {"+0720", 26400, false},
            {"+0730", 27000, false},
            {"+09", 32400, false},
            {"+08", 28800, false},
        }};

        inline constexpr array<zone::transition, 8> transitions{{
            {1, -2177477725},
            {2, -2038200925},
            {3, -1167634800},
            {4, -1073028000},
            {5, -894180000},
            {6, -879665400},
            {5, -767005200},
            {7, 378662400},
        }};
    }

    inline constexpr db::entry singapore{"Asia/Singapore",
                                         "peninsular Malaysia",
                                         "+0117+10351",
                                         detail::singapore::country_codes,
                                         detail::singapore::offsets,
                                         detail::singapore::transitions};

    // clang-format on
}
