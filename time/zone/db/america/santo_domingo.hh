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

    namespace detail::santo_domingo
    {
        inline constexpr array<country::code, 1> country_codes{
            "DO",
        };

        inline constexpr array<zone::offset, 6> offsets{{
            {"LMT", -16776, false},
            {"SDMT", -16800, false},
            {"EDT", -14400, true},
            {"EST", -18000, false},
            {"-0430", -16200, true},
            {"AST", -14400, false},
        }};

        inline constexpr array<zone::transition, 17> transitions{{
            {1, -2524504824},
            {3, -1159773600},
            {2, -100119600},
            {3, -89668800},
            {4, -5770800},
            {3, 4422600},
            {4, 25678800},
            {3, 33193800},
            {4, 57733200},
            {3, 64816200},
            {4, 89182800},
            {3, 96438600},
            {4, 120632400},
            {3, 127974600},
            {5, 152082000},
            {3, 972799200},
            {5, 975823200},
        }};
    }

    inline constexpr db::entry santo_domingo{"America/Santo_Domingo",
                                             "",
                                             "+1828-06954",
                                             detail::santo_domingo::country_codes,
                                             detail::santo_domingo::offsets,
                                             detail::santo_domingo::transitions};

    // clang-format on
}
