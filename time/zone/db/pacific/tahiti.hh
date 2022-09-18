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

    namespace detail::tahiti
    {
        inline constexpr array<country::code, 1> country_codes{
            "PF",
        };

        inline constexpr array<zone::offset, 2> offsets{{
            {"LMT", -35896, false},
            {"-10", -36000, false},
        }};

        inline constexpr array<zone::transition, 2> transitions{{
            {1, -1806674504},
            {1, 2147483647},
        }};
    }

    inline constexpr db::entry tahiti{"Pacific/Tahiti",
                                      "Society Islands",
                                      "-1732-14934",
                                      detail::tahiti::country_codes,
                                      detail::tahiti::offsets,
                                      detail::tahiti::transitions};

    // clang-format on
}
