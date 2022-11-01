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

    namespace detail::belem
    {
        inline constexpr array<country::code, 1> country_codes{
            "BR",
        };

        inline constexpr array<zone::offset, 3> offsets{{
            {"LMT", -11636, false},
            {"-02", -7200, true},
            {"-03", -10800, false},
        }};

        inline constexpr array<zone::transition, 29> transitions{{
            {2, -1767213964},
            {1, -1206957600},
            {2, -1191362400},
            {1, -1175374800},
            {2, -1159826400},
            {1, -633819600},
            {2, -622069200},
            {1, -602283600},
            {2, -591832800},
            {1, -570747600},
            {2, -560210400},
            {1, -539125200},
            {2, -531352800},
            {1, -191365200},
            {2, -184197600},
            {1, -155163600},
            {2, -150069600},
            {1, -128898000},
            {2, -121125600},
            {1, -99954000},
            {2, -89589600},
            {1, -68418000},
            {2, -57967200},
            {1, 499748400},
            {2, 511236000},
            {1, 530593200},
            {2, 540266400},
            {1, 562129200},
            {2, 571197600},
        }};
    }

    inline constexpr db::entry belem{"America/Belem",
                                     "Pará (east); Amapá",
                                     "-0127-04829",
                                     detail::belem::country_codes,
                                     detail::belem::offsets,
                                     detail::belem::transitions};

    // clang-format on
}
