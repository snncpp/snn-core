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

    namespace detail::mexico_city
    {
        inline constexpr array<country::code, 1> country_codes{
            "MX",
        };

        inline constexpr array<zone::offset, 8> offsets{{
            {"LMT", -23796, false},
            {"MST", -25200, false},
            {"CST", -21600, false},
            {"MDT", -21600, true},
            {"MST", -25200, false},
            {"CDT", -18000, true},
            {"CWT", -18000, true},
            {"CST", -21600, false},
        }};

        inline constexpr array<zone::transition, 68> transitions{{
            {1, -1514739600},
            {2, -1343066400},
            {4, -1234807200},
            {3, -1220292000},
            {4, -1207159200},
            {2, -1191344400},
            {5, -975261600},
            {2, -963169200},
            {5, -917114400},
            {2, -907354800},
            {6, -821901600},
            {2, -810068400},
            {5, -627501600},
            {2, -612990000},
            {5, 828864000},
            {2, 846399600},
            {5, 860313600},
            {2, 877849200},
            {5, 891763200},
            {2, 909298800},
            {5, 923212800},
            {2, 941353200},
            {5, 954662400},
            {2, 972802800},
            {5, 989136000},
            {2, 1001833200},
            {5, 1018166400},
            {2, 1035702000},
            {5, 1049616000},
            {2, 1067151600},
            {5, 1081065600},
            {2, 1099206000},
            {5, 1112515200},
            {2, 1130655600},
            {5, 1143964800},
            {2, 1162105200},
            {5, 1175414400},
            {2, 1193554800},
            {5, 1207468800},
            {2, 1225004400},
            {5, 1238918400},
            {2, 1256454000},
            {5, 1270368000},
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

    inline constexpr db::entry mexico_city{"America/Mexico_City",
                                           "Central Mexico",
                                           "+1924-09909",
                                           detail::mexico_city::country_codes,
                                           detail::mexico_city::offsets,
                                           detail::mexico_city::transitions};

    // clang-format on
}
