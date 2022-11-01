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

namespace snn::time::zone::db::australia
{
    // clang-format off

    namespace detail::eucla
    {
        inline constexpr array<country::code, 1> country_codes{
            "AU",
        };

        inline constexpr array<zone::offset, 4> offsets{{
            {"LMT", 30928, false},
            {"+0945", 35100, true},
            {"+0845", 31500, false},
            {"+0845", 31500, false},
        }};

        inline constexpr array<zone::transition, 19> transitions{{
            {3, -2337928528},
            {1, -1672555500},
            {2, -1665384300},
            {1, -883637100},
            {2, -876120300},
            {1, -860395500},
            {2, -844670700},
            {1, 152039700},
            {2, 162926100},
            {1, 436295700},
            {2, 447182100},
            {1, 690311700},
            {2, 699383700},
            {1, 1165079700},
            {2, 1174756500},
            {1, 1193505300},
            {2, 1206810900},
            {1, 1224954900},
            {2, 1238260500},
        }};
    }

    inline constexpr db::entry eucla{"Australia/Eucla",
                                     "Western Australia (Eucla)",
                                     "-3143+12852",
                                     detail::eucla::country_codes,
                                     detail::eucla::offsets,
                                     detail::eucla::transitions};

    // clang-format on
}
