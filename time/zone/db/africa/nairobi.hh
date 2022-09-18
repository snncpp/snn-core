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

namespace snn::time::zone::db::africa
{
    // clang-format off

    namespace detail::nairobi
    {
        inline constexpr array<country::code, 10> country_codes{
            "KE",
            "DJ",
            "ER",
            "ET",
            "KM",
            "MG",
            "SO",
            "TZ",
            "UG",
            "YT",
        };

        inline constexpr array<zone::offset, 5> offsets{{
            {"LMT", 8836, false},
            {"+0230", 9000, false},
            {"EAT", 10800, false},
            {"+0245", 9900, false},
            {"EAT", 10800, false},
        }};

        inline constexpr array<zone::transition, 5> transitions{{
            {1, -1946168836},
            {2, -1309746600},
            {1, -1261969200},
            {3, -1041388200},
            {2, -865305900},
        }};
    }

    inline constexpr db::entry nairobi{"Africa/Nairobi",
                                       "",
                                       "-0117+03649",
                                       detail::nairobi::country_codes,
                                       detail::nairobi::offsets,
                                       detail::nairobi::transitions};

    // clang-format on
}
