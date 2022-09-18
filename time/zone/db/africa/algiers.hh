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

    namespace detail::algiers
    {
        inline constexpr array<country::code, 1> country_codes{
            "DZ",
        };

        inline constexpr array<zone::offset, 8> offsets{{
            {"LMT", 732, false},
            {"PMT", 561, false},
            {"WEST", 3600, true},
            {"WET", 0, false},
            {"WET", 0, false},
            {"CEST", 7200, true},
            {"CET", 3600, false},
            {"WEST", 3600, true},
        }};

        inline constexpr array<zone::transition, 34> transitions{{
            {1, -2486592732},
            {4, -1855958961},
            {2, -1689814800},
            {3, -1680397200},
            {2, -1665363600},
            {3, -1648342800},
            {2, -1635123600},
            {3, -1616893200},
            {2, -1604278800},
            {3, -1585443600},
            {2, -1574038800},
            {3, -1552266000},
            {2, -1539997200},
            {3, -1531443600},
            {2, -956365200},
            {4, -950486400},
            {6, -942012000},
            {5, -812502000},
            {6, -796262400},
            {5, -781052400},
            {6, -766630800},
            {4, -733280400},
            {6, -439430400},
            {4, -212029200},
            {2, 41468400},
            {3, 54774000},
            {7, 231724800},
            {6, 246236400},
            {5, 259545600},
            {6, 275274000},
            {4, 309740400},
            {7, 325468800},
            {4, 341802000},
            {6, 357523200},
        }};
    }

    inline constexpr db::entry algiers{"Africa/Algiers",
                                       "",
                                       "+3647+00303",
                                       detail::algiers::country_codes,
                                       detail::algiers::offsets,
                                       detail::algiers::transitions};

    // clang-format on
}
