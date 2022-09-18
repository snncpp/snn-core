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

    namespace detail::bangkok
    {
        inline constexpr array<country::code, 5> country_codes{
            "TH",
            "CX",
            "KH",
            "LA",
            "VN",
        };

        inline constexpr array<zone::offset, 3> offsets{{
            {"LMT", 24124, false},
            {"BMT", 24124, false},
            {"+07", 25200, false},
        }};

        inline constexpr array<zone::transition, 3> transitions{{
            {1, -2840164924},
            {2, -1570084924},
            {2, 2147483647},
        }};
    }

    inline constexpr db::entry bangkok{"Asia/Bangkok",
                                       "Indochina (most areas)",
                                       "+1345+10031",
                                       detail::bangkok::country_codes,
                                       detail::bangkok::offsets,
                                       detail::bangkok::transitions};

    // clang-format on
}
