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

    namespace detail::pyongyang
    {
        inline constexpr array<country::code, 1> country_codes{
            "KP",
        };

        inline constexpr array<zone::offset, 4> offsets{{
            {"LMT", 30180, false},
            {"KST", 30600, false},
            {"JST", 32400, false},
            {"KST", 32400, false},
        }};

        inline constexpr array<zone::transition, 5> transitions{{
            {1, -1948782180},
            {2, -1830414600},
            {3, -768646800},
            {1, 1439564400},
            {3, 1525446000},
        }};
    }

    inline constexpr db::entry pyongyang{"Asia/Pyongyang",
                                         "",
                                         "+3901+12545",
                                         detail::pyongyang::country_codes,
                                         detail::pyongyang::offsets,
                                         detail::pyongyang::transitions};

    // clang-format on
}
