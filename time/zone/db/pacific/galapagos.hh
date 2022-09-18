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

namespace snn::time::zone::db::pacific
{
    // clang-format off

    namespace detail::galapagos
    {
        inline constexpr array<country::code, 1> country_codes{
            "EC",
        };

        inline constexpr array<zone::offset, 4> offsets{{
            {"LMT", -21504, false},
            {"-05", -18000, false},
            {"-05", -18000, true},
            {"-06", -21600, false},
        }};

        inline constexpr array<zone::transition, 5> transitions{{
            {1, -1230746496},
            {3, 504939600},
            {2, 722930400},
            {3, 728888400},
            {3, 2147483647},
        }};
    }

    inline constexpr db::entry galapagos{"Pacific/Galapagos",
                                         "Galápagos Islands",
                                         "-0054-08936",
                                         detail::galapagos::country_codes,
                                         detail::galapagos::offsets,
                                         detail::galapagos::transitions};

    // clang-format on
}
