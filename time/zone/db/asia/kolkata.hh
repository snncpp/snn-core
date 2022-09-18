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

    namespace detail::kolkata
    {
        inline constexpr array<country::code, 1> country_codes{
            "IN",
        };

        inline constexpr array<zone::offset, 5> offsets{{
            {"LMT", 21208, false},
            {"HMT", 21200, false},
            {"MMT", 19270, false},
            {"IST", 19800, false},
            {"+0630", 23400, true},
        }};

        inline constexpr array<zone::transition, 7> transitions{{
            {1, -3645237208},
            {2, -3155694800},
            {3, -2019705670},
            {4, -891581400},
            {3, -872058600},
            {4, -862637400},
            {3, -764145000},
        }};
    }

    inline constexpr db::entry kolkata{"Asia/Kolkata",
                                       "",
                                       "+2232+08822",
                                       detail::kolkata::country_codes,
                                       detail::kolkata::offsets,
                                       detail::kolkata::transitions};

    // clang-format on
}
