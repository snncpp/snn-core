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

    namespace detail::gambier
    {
        inline constexpr array<country::code, 1> country_codes{
            "PF",
        };

        inline constexpr array<zone::offset, 2> offsets{{
            {"LMT", -32388, false},
            {"-09", -32400, false},
        }};

        inline constexpr array<zone::transition, 2> transitions{{
            {1, -1806678012},
            {1, 2147483647},
        }};
    }

    inline constexpr db::entry gambier{"Pacific/Gambier",
                                       "Gambier Islands",
                                       "-2308-13457",
                                       detail::gambier::country_codes,
                                       detail::gambier::offsets,
                                       detail::gambier::transitions};

    // clang-format on
}
