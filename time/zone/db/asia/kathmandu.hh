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

    namespace detail::kathmandu
    {
        inline constexpr array<country::code, 1> country_codes{
            "NP",
        };

        inline constexpr array<zone::offset, 3> offsets{{
            {"LMT", 20476, false},
            {"+0530", 19800, false},
            {"+0545", 20700, false},
        }};

        inline constexpr array<zone::transition, 2> transitions{{
            {1, -1577943676},
            {2, 504901800},
        }};
    }

    inline constexpr db::entry kathmandu{"Asia/Kathmandu",
                                         "",
                                         "+2743+08519",
                                         detail::kathmandu::country_codes,
                                         detail::kathmandu::offsets,
                                         detail::kathmandu::transitions};

    // clang-format on
}
