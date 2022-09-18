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

    namespace detail::guatemala
    {
        inline constexpr array<country::code, 1> country_codes{
            "GT",
        };

        inline constexpr array<zone::offset, 3> offsets{{
            {"LMT", -21724, false},
            {"CDT", -18000, true},
            {"CST", -21600, false},
        }};

        inline constexpr array<zone::transition, 9> transitions{{
            {2, -1617040676},
            {1, 123055200},
            {2, 130914000},
            {1, 422344800},
            {2, 433054800},
            {1, 669708000},
            {2, 684219600},
            {1, 1146376800},
            {2, 1159678800},
        }};
    }

    inline constexpr db::entry guatemala{"America/Guatemala",
                                         "",
                                         "+1438-09031",
                                         detail::guatemala::country_codes,
                                         detail::guatemala::offsets,
                                         detail::guatemala::transitions};

    // clang-format on
}
