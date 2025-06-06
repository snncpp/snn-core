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
            {"MDT", -21600, true},
            {"MST", -25200, false},
            {"CDT", -18000, true},
            {"CST", -21600, false},
        }};

        inline constexpr array<zone::transition, 61> transitions{{
            {1, -1514739600},
            {2, -1343149200},
            {4, -1234807200},
            {3, -1220461200},
            {4, -1207159200},
            {2, -1191344400},
            {4, -873828000},
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
        }};
    }

    inline constexpr db::entry bahia_banderas{"America/Bahia_Banderas",
                                              "Bahía de Banderas",
                                              "+2048-10515",
                                              detail::bahia_banderas::country_codes,
                                              detail::bahia_banderas::offsets,
                                              detail::bahia_banderas::transitions};

    // clang-format on
}
