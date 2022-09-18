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

    namespace detail::chagos
    {
        inline constexpr array<country::code, 1> country_codes{
            "IO",
        };

        inline constexpr array<zone::offset, 3> offsets{{
            {"LMT", 17380, false},
            {"+05", 18000, false},
            {"+06", 21600, false},
        }};

        inline constexpr array<zone::transition, 3> transitions{{
            {1, -1988167780},
            {2, 820436400},
            {2, 2147483647},
        }};
    }

    inline constexpr db::entry chagos{"Indian/Chagos",
                                      "",
                                      "-0720+07225",
                                      detail::chagos::country_codes,
                                      detail::chagos::offsets,
                                      detail::chagos::transitions};

    // clang-format on
}
