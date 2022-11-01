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

    namespace detail::yangon
    {
        inline constexpr array<country::code, 2> country_codes{
            "MM",
            "CC",
        };

        inline constexpr array<zone::offset, 5> offsets{{
            {"LMT", 23087, false},
            {"RMT", 23087, false},
            {"+0630", 23400, false},
            {"+09", 32400, false},
            {"+0630", 23400, false},
        }};

        inline constexpr array<zone::transition, 4> transitions{{
            {1, -2840163887},
            {2, -1577946287},
            {3, -873268200},
            {2, -778410000},
        }};
    }

    inline constexpr db::entry yangon{"Asia/Yangon",
                                      "",
                                      "+1647+09610",
                                      detail::yangon::country_codes,
                                      detail::yangon::offsets,
                                      detail::yangon::transitions};

    // clang-format on
}
