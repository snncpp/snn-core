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

namespace snn::time::zone::db::america
{
    // clang-format off

    namespace detail::el_salvador
    {
        inline constexpr array<country::code, 1> country_codes{
            "SV",
        };

        inline constexpr array<zone::offset, 3> offsets{{
            {"LMT", -21408, false},
            {"CDT", -18000, true},
            {"CST", -21600, false},
        }};

        inline constexpr array<zone::transition, 5> transitions{{
            {2, -1546279392},
            {1, 547020000},
            {2, 559717200},
            {1, 578469600},
            {2, 591166800},
        }};
    }

    inline constexpr db::entry el_salvador{"America/El_Salvador",
                                           "",
                                           "+1342-08912",
                                           detail::el_salvador::country_codes,
                                           detail::el_salvador::offsets,
                                           detail::el_salvador::transitions};

    // clang-format on
}
