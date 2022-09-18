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

    namespace detail::martinique
    {
        inline constexpr array<country::code, 1> country_codes{
            "MQ",
        };

        inline constexpr array<zone::offset, 4> offsets{{
            {"LMT", -14660, false},
            {"FFMT", -14660, false},
            {"AST", -14400, false},
            {"ADT", -10800, true},
        }};

        inline constexpr array<zone::transition, 4> transitions{{
            {1, -2524506940},
            {2, -1851537340},
            {3, 323841600},
            {2, 338958000},
        }};
    }

    inline constexpr db::entry martinique{"America/Martinique",
                                          "",
                                          "+1436-06105",
                                          detail::martinique::country_codes,
                                          detail::martinique::offsets,
                                          detail::martinique::transitions};

    // clang-format on
}
