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

    namespace detail::merida
    {
        inline constexpr array<country::code, 1> country_codes{
            "MX",
        };

        inline constexpr array<zone::offset, 5> offsets{{
            {"LMT", -21508, false},
            {"CST", -21600, false},
            {"EST", -18000, false},
            {"CDT", -18000, true},
            {"CST", -21600, false},
        }};

        inline constexpr array<zone::transition, 87> transitions{{
            {1, -1514743200},
            {2, 377935200},
            {1, 407653200},
            {3, 828864000},
            {1, 846399600},
            {3, 860313600},
            {1, 877849200},
            {3, 891763200},
            {1, 909298800},
            {3, 923212800},
            {1, 941353200},
            {3, 954662400},
            {1, 972802800},
            {3, 989136000},
            {1, 1001833200},
            {3, 1018166400},
            {1, 1035702000},
            {3, 1049616000},
            {1, 1067151600},
            {3, 1081065600},
            {1, 1099206000},
            {3, 1112515200},
            {1, 1130655600},
            {3, 1143964800},
            {1, 1162105200},
            {3, 1175414400},
            {1, 1193554800},
            {3, 1207468800},
            {1, 1225004400},
            {3, 1238918400},
            {1, 1256454000},
            {3, 1270368000},
            {1, 1288508400},
            {3, 1301817600},
            {1, 1319958000},
            {3, 1333267200},
            {1, 1351407600},
            {3, 1365321600},
            {1, 1382857200},
            {3, 1396771200},
            {1, 1414306800},
            {3, 1428220800},
            {1, 1445756400},
            {3, 1459670400},
            {1, 1477810800},
            {3, 1491120000},
            {1, 1509260400},
            {3, 1522569600},
            {1, 1540710000},
            {3, 1554624000},
            {1, 1572159600},
            {3, 1586073600},
            {1, 1603609200},
            {3, 1617523200},
            {1, 1635663600},
            {3, 1648972800},
            {1, 1667113200},
            {3, 1680422400},
            {1, 1698562800},
            {3, 1712476800},
            {1, 1730012400},
            {3, 1743926400},
            {1, 1761462000},
            {3, 1775376000},
            {1, 1792911600},
            {3, 1806825600},
            {1, 1824966000},
            {3, 1838275200},
            {1, 1856415600},
            {3, 1869724800},
            {1, 1887865200},
            {3, 1901779200},
            {1, 1919314800},
            {3, 1933228800},
            {1, 1950764400},
            {3, 1964678400},
            {1, 1982818800},
            {3, 1996128000},
            {1, 2014268400},
            {3, 2027577600},
            {1, 2045718000},
            {3, 2059027200},
            {1, 2077167600},
            {3, 2091081600},
            {1, 2108617200},
            {3, 2122531200},
            {1, 2140066800},
        }};
    }

    inline constexpr db::entry merida{"America/Merida",
                                      "Central Time - Campeche, Yucatán",
                                      "+2058-08937",
                                      detail::merida::country_codes,
                                      detail::merida::offsets,
                                      detail::merida::transitions};

    // clang-format on
}
