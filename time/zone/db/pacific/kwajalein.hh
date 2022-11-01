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

namespace snn::time::zone::db::pacific
{
    // clang-format off

    namespace detail::kwajalein
    {
        inline constexpr array<country::code, 1> country_codes{
            "MH",
        };

        inline constexpr array<zone::offset, 6> offsets{{
            {"LMT", 40160, false},
            {"+11", 39600, false},
            {"+10", 36000, false},
            {"+09", 32400, false},
            {"-12", -43200, false},
            {"+12", 43200, false},
        }};

        inline constexpr array<zone::transition, 6> transitions{{
            {1, -2177492960},
            {2, -1041418800},
            {3, -907408800},
            {1, -817462800},
            {4, -7988400},
            {5, 745934400},
        }};
    }

    inline constexpr db::entry kwajalein{"Pacific/Kwajalein",
                                         "Kwajalein",
                                         "+0905+16720",
                                         detail::kwajalein::country_codes,
                                         detail::kwajalein::offsets,
                                         detail::kwajalein::transitions};

    // clang-format on
}
