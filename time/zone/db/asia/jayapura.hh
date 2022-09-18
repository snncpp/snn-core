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

    namespace detail::jayapura
    {
        inline constexpr array<country::code, 1> country_codes{
            "ID",
        };

        inline constexpr array<zone::offset, 4> offsets{{
            {"LMT", 33768, false},
            {"+09", 32400, false},
            {"+0930", 34200, false},
            {"WIT", 32400, false},
        }};

        inline constexpr array<zone::transition, 3> transitions{{
            {1, -1172913768},
            {2, -799491600},
            {3, -189423000},
        }};
    }

    inline constexpr db::entry jayapura{"Asia/Jayapura",
                                        "New Guinea (West Papua / Irian Jaya); Malukus/Moluccas",
                                        "-0232+14042",
                                        detail::jayapura::country_codes,
                                        detail::jayapura::offsets,
                                        detail::jayapura::transitions};

    // clang-format on
}
