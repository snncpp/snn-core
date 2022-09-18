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

    namespace detail::niue
    {
        inline constexpr array<country::code, 1> country_codes{
            "NU",
        };

        inline constexpr array<zone::offset, 3> offsets{{
            {"LMT", -40780, false},
            {"-1120", -40800, false},
            {"-11", -39600, false},
        }};

        inline constexpr array<zone::transition, 3> transitions{{
            {1, -543069620},
            {2, -173623200},
            {2, 2147483647},
        }};
    }

    inline constexpr db::entry niue{"Pacific/Niue",
                                    "",
                                    "-1901-16955",
                                    detail::niue::country_codes,
                                    detail::niue::offsets,
                                    detail::niue::transitions};

    // clang-format on
}
