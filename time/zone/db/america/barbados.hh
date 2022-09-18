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

    namespace detail::barbados
    {
        inline constexpr array<country::code, 1> country_codes{
            "BB",
        };

        inline constexpr array<zone::offset, 6> offsets{{
            {"LMT", -14309, false},
            {"ADT", -10800, true},
            {"AST", -14400, false},
            {"AST", -14400, false},
            {"-0330", -12600, true},
            {"ADT", -10800, true},
        }};

        inline constexpr array<zone::transition, 15> transitions{{
            {3, -1841256091},
            {1, -874263600},
            {2, -862682400},
            {1, -841604400},
            {2, -830714400},
            {4, -811882800},
            {2, -798660000},
            {5, 234943200},
            {3, 244616400},
            {5, 261554400},
            {3, 276066000},
            {5, 293004000},
            {3, 307515600},
            {5, 325058400},
            {3, 338706000},
        }};
    }

    inline constexpr db::entry barbados{"America/Barbados",
                                        "",
                                        "+1306-05937",
                                        detail::barbados::country_codes,
                                        detail::barbados::offsets,
                                        detail::barbados::transitions};

    // clang-format on
}
