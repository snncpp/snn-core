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

    namespace detail::chihuahua
    {
        inline constexpr array<country::code, 1> country_codes{
            "MX",
        };

        inline constexpr array<zone::offset, 6> offsets{{
            {"LMT", -25460, false},
            {"MST", -25200, false},
            {"CST", -21600, false},
            {"CDT", -18000, true},
            {"MDT", -21600, true},
            {"MST", -25200, false},
        }};

        inline constexpr array<zone::transition, 90> transitions{{
            {1, -1514739600},
            {2, -1343066400},
            {1, -1234807200},
            {2, -1220292000},
            {1, -1207159200},
            {2, -1191344400},
            {3, 828864000},
            {2, 846399600},
            {3, 860313600},
            {2, 877849200},
            {4, 891766800},
            {1, 909302400},
            {4, 923216400},
            {1, 941356800},
            {4, 954666000},
            {1, 972806400},
            {4, 989139600},
            {1, 1001836800},
            {4, 1018170000},
            {1, 1035705600},
            {4, 1049619600},
            {1, 1067155200},
            {4, 1081069200},
            {1, 1099209600},
            {4, 1112518800},
            {1, 1130659200},
            {4, 1143968400},
            {1, 1162108800},
            {4, 1175418000},
            {1, 1193558400},
            {4, 1207472400},
            {1, 1225008000},
            {4, 1238922000},
            {1, 1256457600},
            {4, 1270371600},
            {1, 1288512000},
            {4, 1301821200},
            {1, 1319961600},
            {4, 1333270800},
            {1, 1351411200},
            {4, 1365325200},
            {1, 1382860800},
            {4, 1396774800},
            {1, 1414310400},
            {4, 1428224400},
            {1, 1445760000},
            {4, 1459674000},
            {1, 1477814400},
            {4, 1491123600},
            {1, 1509264000},
            {4, 1522573200},
            {1, 1540713600},
            {4, 1554627600},
            {1, 1572163200},
            {4, 1586077200},
            {1, 1603612800},
            {4, 1617526800},
            {1, 1635667200},
            {4, 1648976400},
            {1, 1667116800},
            {4, 1680426000},
            {1, 1698566400},
            {4, 1712480400},
            {1, 1730016000},
            {4, 1743930000},
            {1, 1761465600},
            {4, 1775379600},
            {1, 1792915200},
            {4, 1806829200},
            {1, 1824969600},
            {4, 1838278800},
            {1, 1856419200},
            {4, 1869728400},
            {1, 1887868800},
            {4, 1901782800},
            {1, 1919318400},
            {4, 1933232400},
            {1, 1950768000},
            {4, 1964682000},
            {1, 1982822400},
            {4, 1996131600},
            {1, 2014272000},
            {4, 2027581200},
            {1, 2045721600},
            {4, 2059030800},
            {1, 2077171200},
            {4, 2091085200},
            {1, 2108620800},
            {4, 2122534800},
            {1, 2140070400},
        }};
    }

    inline constexpr db::entry chihuahua{"America/Chihuahua",
                                         "Mountain Time - Chihuahua (most areas)",
                                         "+2838-10605",
                                         detail::chihuahua::country_codes,
                                         detail::chihuahua::offsets,
                                         detail::chihuahua::transitions};

    // clang-format on
}
