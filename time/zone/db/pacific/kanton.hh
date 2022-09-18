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

    namespace detail::kanton
    {
        inline constexpr array<country::code, 1> country_codes{
            "KI",
        };

        inline constexpr array<zone::offset, 4> offsets{{
            {"-00", 0, false},
            {"-12", -43200, false},
            {"-11", -39600, false},
            {"+13", 46800, false},
        }};

        inline constexpr array<zone::transition, 4> transitions{{
            {1, -1020470400},
            {2, 307627200},
            {3, 788871600},
            {3, 2147483647},
        }};
    }

    inline constexpr db::entry kanton{"Pacific/Kanton",
                                      "Phoenix Islands",
                                      "-0247-17143",
                                      detail::kanton::country_codes,
                                      detail::kanton::offsets,
                                      detail::kanton::transitions};

    // clang-format on
}
