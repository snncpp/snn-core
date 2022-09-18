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

    namespace detail::rio_branco
    {
        inline constexpr array<country::code, 1> country_codes{
            "BR",
        };

        inline constexpr array<zone::offset, 5> offsets{{
            {"LMT", -16272, false},
            {"-04", -14400, true},
            {"-05", -18000, false},
            {"-04", -14400, false},
            {"-05", -18000, false},
        }};

        inline constexpr array<zone::transition, 32> transitions{{
            {2, -1767209328},
            {1, -1206950400},
            {2, -1191355200},
            {1, -1175367600},
            {2, -1159819200},
            {1, -633812400},
            {2, -622062000},
            {1, -602276400},
            {2, -591825600},
            {1, -570740400},
            {2, -560203200},
            {1, -539118000},
            {2, -531345600},
            {1, -191358000},
            {2, -184190400},
            {1, -155156400},
            {2, -150062400},
            {1, -128890800},
            {2, -121118400},
            {1, -99946800},
            {2, -89582400},
            {1, -68410800},
            {2, -57960000},
            {1, 499755600},
            {2, 511243200},
            {1, 530600400},
            {2, 540273600},
            {1, 562136400},
            {2, 571204800},
            {3, 1214283600},
            {2, 1384056000},
            {2, 2147483647},
        }};
    }

    inline constexpr db::entry rio_branco{"America/Rio_Branco",
                                          "Acre",
                                          "-0958-06748",
                                          detail::rio_branco::country_codes,
                                          detail::rio_branco::offsets,
                                          detail::rio_branco::transitions};

    // clang-format on
}
