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

namespace snn::time::zone::db::asia
{
    // clang-format off

    namespace detail::dili
    {
        inline constexpr array<country::code, 1> country_codes{
            "TL",
        };

        inline constexpr array<zone::offset, 3> offsets{{
            {"LMT", 30140, false},
            {"+08", 28800, false},
            {"+09", 32400, false},
        }};

        inline constexpr array<zone::transition, 5> transitions{{
            {1, -1830414140},
            {2, -879152400},
            {1, 199897200},
            {2, 969120000},
            {2, 2147483647},
        }};
    }

    inline constexpr db::entry dili{"Asia/Dili",
                                    "",
                                    "-0833+12535",
                                    detail::dili::country_codes,
                                    detail::dili::offsets,
                                    detail::dili::transitions};

    // clang-format on
}
