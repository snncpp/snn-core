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

    namespace detail::windhoek
    {
        inline constexpr array<country::code, 1> country_codes{
            "NA",
        };

        inline constexpr array<zone::offset, 6> offsets{{
            {"LMT", 4104, false},
            {"+0130", 5400, false},
            {"SAST", 7200, false},
            {"SAST", 10800, true},
            {"WAT", 3600, true},
            {"CAT", 7200, false},
        }};

        inline constexpr array<zone::transition, 53> transitions{{
            {1, -2458170504},
            {2, -2109288600},
            {3, -860976000},
            {2, -845254800},
            {5, 637970400},
            {4, 764200800},
            {5, 778640400},
            {4, 796780800},
            {5, 810090000},
            {4, 828835200},
            {5, 841539600},
            {4, 860284800},
            {5, 873594000},
            {4, 891734400},
            {5, 905043600},
            {4, 923184000},
            {5, 936493200},
            {4, 954633600},
            {5, 967942800},
            {4, 986083200},
            {5, 999392400},
            {4, 1018137600},
            {5, 1030842000},
            {4, 1049587200},
            {5, 1062896400},
            {4, 1081036800},
            {5, 1094346000},
            {4, 1112486400},
            {5, 1125795600},
            {4, 1143936000},
            {5, 1157245200},
            {4, 1175385600},
            {5, 1188694800},
            {4, 1207440000},
            {5, 1220749200},
            {4, 1238889600},
            {5, 1252198800},
            {4, 1270339200},
            {5, 1283648400},
            {4, 1301788800},
            {5, 1315098000},
            {4, 1333238400},
            {5, 1346547600},
            {4, 1365292800},
            {5, 1377997200},
            {4, 1396742400},
            {5, 1410051600},
            {4, 1428192000},
            {5, 1441501200},
            {4, 1459641600},
            {5, 1472950800},
            {4, 1491091200},
            {5, 1504400400},
        }};
    }

    inline constexpr db::entry windhoek{"Africa/Windhoek",
                                        "",
                                        "-2234+01706",
                                        detail::windhoek::country_codes,
                                        detail::windhoek::offsets,
                                        detail::windhoek::transitions};

    // clang-format on
}
