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

    namespace detail::rarotonga
    {
        inline constexpr array<country::code, 1> country_codes{
            "CK",
        };

        inline constexpr array<zone::offset, 5> offsets{{
            {"LMT", 48056, false},
            {"LMT", -38344, false},
            {"-1030", -37800, false},
            {"-10", -36000, false},
            {"-0930", -34200, true},
        }};

        inline constexpr array<zone::transition, 29> transitions{{
            {1, -2209555256},
            {2, -543072056},
            {4, 279714600},
            {3, 289387800},
            {4, 309952800},
            {3, 320837400},
            {4, 341402400},
            {3, 352287000},
            {4, 372852000},
            {3, 384341400},
            {4, 404906400},
            {3, 415791000},
            {4, 436356000},
            {3, 447240600},
            {4, 467805600},
            {3, 478690200},
            {4, 499255200},
            {3, 510139800},
            {4, 530704800},
            {3, 541589400},
            {4, 562154400},
            {3, 573643800},
            {4, 594208800},
            {3, 605093400},
            {4, 625658400},
            {3, 636543000},
            {4, 657108000},
            {3, 667992600},
            {3, 2147483647},
        }};
    }

    inline constexpr db::entry rarotonga{"Pacific/Rarotonga",
                                         "",
                                         "-2114-15946",
                                         detail::rarotonga::country_codes,
                                         detail::rarotonga::offsets,
                                         detail::rarotonga::transitions};

    // clang-format on
}
