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

    namespace detail::nauru
    {
        inline constexpr array<country::code, 1> country_codes{
            "NR",
        };

        inline constexpr array<zone::offset, 4> offsets{{
            {"LMT", 40060, false},
            {"+1130", 41400, false},
            {"+09", 32400, false},
            {"+12", 43200, false},
        }};

        inline constexpr array<zone::transition, 4> transitions{{
            {1, -1545131260},
            {2, -862918200},
            {1, -767350800},
            {3, 287418600},
        }};
    }

    inline constexpr db::entry nauru{"Pacific/Nauru",
                                     "",
                                     "-0031+16655",
                                     detail::nauru::country_codes,
                                     detail::nauru::offsets,
                                     detail::nauru::transitions};

    // clang-format on
}
