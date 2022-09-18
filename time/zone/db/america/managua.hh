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

    namespace detail::managua
    {
        inline constexpr array<country::code, 1> country_codes{
            "NI",
        };

        inline constexpr array<zone::offset, 6> offsets{{
            {"LMT", -20708, false},
            {"MMT", -20712, false},
            {"CST", -21600, false},
            {"EST", -18000, false},
            {"CDT", -18000, true},
            {"CST", -21600, false},
        }};

        inline constexpr array<zone::transition, 16> transitions{{
            {1, -2524500892},
            {2, -1121105688},
            {3, 105084000},
            {2, 161758800},
            {4, 290584800},
            {2, 299134800},
            {4, 322034400},
            {2, 330584400},
            {3, 694260000},
            {2, 717310800},
            {3, 725868000},
            {2, 852094800},
            {4, 1113112800},
            {2, 1128229200},
            {4, 1146384000},
            {2, 1159682400},
        }};
    }

    inline constexpr db::entry managua{"America/Managua",
                                       "",
                                       "+1209-08617",
                                       detail::managua::country_codes,
                                       detail::managua::offsets,
                                       detail::managua::transitions};

    // clang-format on
}
