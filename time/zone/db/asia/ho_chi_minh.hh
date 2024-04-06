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

    namespace detail::ho_chi_minh
    {
        inline constexpr array<country::code, 1> country_codes{
            "VN",
        };

        inline constexpr array<zone::offset, 6> offsets{{
            {"LMT", 25590, false},
            {"PLMT", 25590, false},
            {"+07", 25200, false},
            {"+08", 28800, false},
            {"+09", 32400, false},
            {"+07", 25200, false},
        }};

        inline constexpr array<zone::transition, 9> transitions{{
            {1, -2004073590},
            {2, -1851577590},
            {3, -852105600},
            {4, -782643600},
            {2, -767869200},
            {3, -718095600},
            {2, -457772400},
            {3, -315648000},
            {2, 171820800},
        }};
    }

    inline constexpr db::entry ho_chi_minh{"Asia/Ho_Chi_Minh",
                                           "south Vietnam",
                                           "+1045+10640",
                                           detail::ho_chi_minh::country_codes,
                                           detail::ho_chi_minh::offsets,
                                           detail::ho_chi_minh::transitions};

    // clang-format on
}
