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

namespace snn::time::zone::db::africa
{
    // clang-format off

    namespace detail::ndjamena
    {
        inline constexpr array<country::code, 1> country_codes{
            "TD",
        };

        inline constexpr array<zone::offset, 3> offsets{{
            {"LMT", 3612, false},
            {"WAT", 3600, false},
            {"WAST", 7200, true},
        }};

        inline constexpr array<zone::transition, 3> transitions{{
            {1, -1830387612},
            {2, 308703600},
            {1, 321314400},
        }};
    }

    inline constexpr db::entry ndjamena{"Africa/Ndjamena",
                                        "",
                                        "+1207+01503",
                                        detail::ndjamena::country_codes,
                                        detail::ndjamena::offsets,
                                        detail::ndjamena::transitions};

    // clang-format on
}
