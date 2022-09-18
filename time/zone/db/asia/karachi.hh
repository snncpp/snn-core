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

    namespace detail::karachi
    {
        inline constexpr array<country::code, 1> country_codes{
            "PK",
        };

        inline constexpr array<zone::offset, 6> offsets{{
            {"LMT", 16092, false},
            {"+0530", 19800, false},
            {"+0630", 23400, true},
            {"+05", 18000, false},
            {"PKST", 21600, true},
            {"PKT", 18000, false},
        }};

        inline constexpr array<zone::transition, 11> transitions{{
            {1, -1988166492},
            {2, -862637400},
            {1, -764145000},
            {3, -576135000},
            {5, 38775600},
            {4, 1018119600},
            {5, 1033840800},
            {4, 1212260400},
            {5, 1225476000},
            {4, 1239735600},
            {5, 1257012000},
        }};
    }

    inline constexpr db::entry karachi{"Asia/Karachi",
                                       "",
                                       "+2452+06703",
                                       detail::karachi::country_codes,
                                       detail::karachi::offsets,
                                       detail::karachi::transitions};

    // clang-format on
}
