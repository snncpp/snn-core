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

namespace snn::time::zone::db::antarctica
{
    // clang-format off

    namespace detail::rothera
    {
        inline constexpr array<country::code, 1> country_codes{
            "AQ",
        };

        inline constexpr array<zone::offset, 2> offsets{{
            {"-00", 0, false},
            {"-03", -10800, false},
        }};

        inline constexpr array<zone::transition, 1> transitions{{
            {1, 218246400},
        }};
    }

    inline constexpr db::entry rothera{"Antarctica/Rothera",
                                       "Rothera",
                                       "-6734-06808",
                                       detail::rothera::country_codes,
                                       detail::rothera::offsets,
                                       detail::rothera::transitions};

    // clang-format on
}
