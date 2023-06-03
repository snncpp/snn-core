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

namespace snn::time::zone::db::indian
{
    // clang-format off

    namespace detail::maldives
    {
        inline constexpr array<country::code, 2> country_codes{
            "MV",
            "TF",
        };

        inline constexpr array<zone::offset, 3> offsets{{
            {"LMT", 17640, false},
            {"MMT", 17640, false},
            {"+05", 18000, false},
        }};

        inline constexpr array<zone::transition, 2> transitions{{
            {1, -2840158440},
            {2, -315636840},
        }};
    }

    inline constexpr db::entry maldives{"Indian/Maldives",
                                        "Kerguelen, St Paul I, Amsterdam I",
                                        "+0410+07330",
                                        detail::maldives::country_codes,
                                        detail::maldives::offsets,
                                        detail::maldives::transitions};

    // clang-format on
}
