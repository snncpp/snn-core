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

    namespace detail::bahia_banderas
    {
        inline constexpr array<country::code, 1> country_codes{
            "MX",
        };

        inline constexpr array<zone::offset, 7> offsets{{
            {"LMT", -25260, false},
            {"MST", -25200, false},
            {"CST", -21600, false},
            {"PST", -28800, false},
            {"MDT", -21600, true},
            {"CDT", -18000, true},
            {"CST", -21600, false},
        }};

        inline constexpr array<zone::transition, 93> transitions{{
            {1, -1514739600},
            {2, -1343066400},
            {1, -1234807200},
            {2, -1220292000},
            {1, -1207159200},
            {2, -1191344400},
            {1, -873828000},
            {3, -661539600},
            {1, 28800},
            {4, 828867600},
            {1, 846403200},
            {4, 860317200},
            {1, 877852800},
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
            {5, 1270371600},
            {2, 1288508400},
            {5, 1301817600},
            {2, 1319958000},
            {5, 1333267200},
            {2, 1351407600},
            {5, 1365321600},
            {2, 1382857200},
            {5, 1396771200},
            {2, 1414306800},
            {5, 1428220800},
            {2, 1445756400},
            {5, 1459670400},
            {2, 1477810800},
            {5, 1491120000},
            {2, 1509260400},
            {5, 1522569600},
            {2, 1540710000},
            {5, 1554624000},
            {2, 1572159600},
            {5, 1586073600},
            {2, 1603609200},
            {5, 1617523200},
            {2, 1635663600},
            {5, 1648972800},
            {2, 1667113200},
            {5, 1680422400},
            {2, 1698562800},
            {5, 1712476800},
            {2, 1730012400},
            {5, 1743926400},
            {2, 1761462000},
            {5, 1775376000},
            {2, 1792911600},
            {5, 1806825600},
            {2, 1824966000},
            {5, 1838275200},
            {2, 1856415600},
            {5, 1869724800},
            {2, 1887865200},
            {5, 1901779200},
            {2, 1919314800},
            {5, 1933228800},
            {2, 1950764400},
            {5, 1964678400},
            {2, 1982818800},
            {5, 1996128000},
            {2, 2014268400},
            {5, 2027577600},
            {2, 2045718000},
            {5, 2059027200},
            {2, 2077167600},
            {5, 2091081600},
            {2, 2108617200},
            {5, 2122531200},
            {2, 2140066800},
        }};
    }

    inline constexpr db::entry bahia_banderas{"America/Bahia_Banderas",
                                              "Central Time - Bahía de Banderas",
                                              "+2048-10515",
                                              detail::bahia_banderas::country_codes,
                                              detail::bahia_banderas::offsets,
                                              detail::bahia_banderas::transitions};

    // clang-format on
}
