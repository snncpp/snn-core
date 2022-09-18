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

    namespace detail::lima
    {
        inline constexpr array<country::code, 1> country_codes{
            "PE",
        };

        inline constexpr array<zone::offset, 4> offsets{{
            {"LMT", -18492, false},
            {"LMT", -18516, false},
            {"-04", -14400, true},
            {"-05", -18000, false},
        }};

        inline constexpr array<zone::transition, 17> transitions{{
            {1, -2524503108},
            {3, -1938538284},
            {2, -1009825200},
            {3, -1002052800},
            {2, -986756400},
            {3, -971035200},
            {2, -955306800},
            {3, -939585600},
            {2, 504939600},
            {3, 512712000},
            {2, 536475600},
            {3, 544248000},
            {2, 631170000},
            {3, 638942400},
            {2, 757400400},
            {3, 765172800},
            {3, 2147483647},
        }};
    }

    inline constexpr db::entry lima{"America/Lima",
                                    "",
                                    "-1203-07703",
                                    detail::lima::country_codes,
                                    detail::lima::offsets,
                                    detail::lima::transitions};

    // clang-format on
}
