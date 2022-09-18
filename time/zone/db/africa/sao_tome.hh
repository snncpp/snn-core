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

    namespace detail::sao_tome
    {
        inline constexpr array<country::code, 1> country_codes{
            "ST",
        };

        inline constexpr array<zone::offset, 5> offsets{{
            {"LMT", 1616, false},
            {"LMT", -2205, false},
            {"GMT", 0, false},
            {"WAT", 3600, false},
            {"GMT", 0, false},
        }};

        inline constexpr array<zone::transition, 4> transitions{{
            {1, -2713912016},
            {2, -1830384000},
            {3, 1514768400},
            {4, 1546304400},
        }};
    }

    inline constexpr db::entry sao_tome{"Africa/Sao_Tome",
                                        "",
                                        "+0020+00644",
                                        detail::sao_tome::country_codes,
                                        detail::sao_tome::offsets,
                                        detail::sao_tome::transitions};

    // clang-format on
}
