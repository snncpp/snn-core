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

    namespace detail::mazatlan
    {
        inline constexpr array<country::code, 1> country_codes{
            "MX",
        };

        inline constexpr array<zone::offset, 7> offsets{{
            {"LMT", -25540, false},
            {"MST", -25200, false},
            {"CST", -21600, false},
            {"MDT", -21600, true},
            {"MST", -25200, false},
            {"PST", -28800, false},
            {"MST", -25200, false},
        }};

        inline constexpr array<zone::transition, 93> transitions{{
            {1, -1514739600},
            {2, -1343066400},
            {4, -1234807200},
            {3, -1220292000},
            {4, -1207159200},
            {2, -1191344400},
            {4, -873828000},
            {5, -661539600},
            {4, 28800},
            {3, 828867600},
            {4, 846403200},
            {3, 860317200},
            {4, 877852800},
            {3, 891766800},
            {4, 909302400},
            {3, 923216400},
            {4, 941356800},
            {3, 954666000},
            {4, 972806400},
            {3, 989139600},
            {4, 1001836800},
            {3, 1018170000},
            {4, 1035705600},
            {3, 1049619600},
            {4, 1067155200},
            {3, 1081069200},
            {4, 1099209600},
            {3, 1112518800},
            {4, 1130659200},
            {3, 1143968400},
            {4, 1162108800},
            {3, 1175418000},
            {4, 1193558400},
            {3, 1207472400},
            {4, 1225008000},
            {3, 1238922000},
            {4, 1256457600},
            {3, 1270371600},
            {4, 1288512000},
            {3, 1301821200},
            {4, 1319961600},
            {3, 1333270800},
            {4, 1351411200},
            {3, 1365325200},
            {4, 1382860800},
            {3, 1396774800},
            {4, 1414310400},
            {3, 1428224400},
            {4, 1445760000},
            {3, 1459674000},
            {4, 1477814400},
            {3, 1491123600},
            {4, 1509264000},
            {3, 1522573200},
            {4, 1540713600},
            {3, 1554627600},
            {4, 1572163200},
            {3, 1586077200},
            {4, 1603612800},
            {3, 1617526800},
            {4, 1635667200},
            {3, 1648976400},
            {4, 1667116800},
            {3, 1680426000},
            {4, 1698566400},
            {3, 1712480400},
            {4, 1730016000},
            {3, 1743930000},
            {4, 1761465600},
            {3, 1775379600},
            {4, 1792915200},
            {3, 1806829200},
            {4, 1824969600},
            {3, 1838278800},
            {4, 1856419200},
            {3, 1869728400},
            {4, 1887868800},
            {3, 1901782800},
            {4, 1919318400},
            {3, 1933232400},
            {4, 1950768000},
            {3, 1964682000},
            {4, 1982822400},
            {3, 1996131600},
            {4, 2014272000},
            {3, 2027581200},
            {4, 2045721600},
            {3, 2059030800},
            {4, 2077171200},
            {3, 2091085200},
            {4, 2108620800},
            {3, 2122534800},
            {4, 2140070400},
        }};
    }

    inline constexpr db::entry mazatlan{"America/Mazatlan",
                                        "Mountain Time - Baja California Sur, Nayarit, Sinaloa",
                                        "+2313-10625",
                                        detail::mazatlan::country_codes,
                                        detail::mazatlan::offsets,
                                        detail::mazatlan::transitions};

    // clang-format on
}
