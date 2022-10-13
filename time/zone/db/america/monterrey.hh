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

    namespace detail::monterrey
    {
        inline constexpr array<country::code, 1> country_codes{
            "MX",
        };

        inline constexpr array<zone::offset, 4> offsets{{
            {"LMT", -24076, false},
            {"CST", -21600, false},
            {"CDT", -18000, true},
            {"CST", -21600, false},
        }};

        inline constexpr array<zone::transition, 87> transitions{{
            {1, -1514743200},
            {2, 576057600},
            {3, 594198000},
            {2, 828864000},
            {3, 846399600},
            {2, 860313600},
            {3, 877849200},
            {2, 891763200},
            {3, 909298800},
            {2, 923212800},
            {3, 941353200},
            {2, 954662400},
            {3, 972802800},
            {2, 989136000},
            {3, 1001833200},
            {2, 1018166400},
            {3, 1035702000},
            {2, 1049616000},
            {3, 1067151600},
            {2, 1081065600},
            {3, 1099206000},
            {2, 1112515200},
            {3, 1130655600},
            {2, 1143964800},
            {3, 1162105200},
            {2, 1175414400},
            {3, 1193554800},
            {2, 1207468800},
            {3, 1225004400},
            {2, 1238918400},
            {3, 1256454000},
            {2, 1270368000},
            {3, 1288508400},
            {2, 1301817600},
            {3, 1319958000},
            {2, 1333267200},
            {3, 1351407600},
            {2, 1365321600},
            {3, 1382857200},
            {2, 1396771200},
            {3, 1414306800},
            {2, 1428220800},
            {3, 1445756400},
            {2, 1459670400},
            {3, 1477810800},
            {2, 1491120000},
            {3, 1509260400},
            {2, 1522569600},
            {3, 1540710000},
            {2, 1554624000},
            {3, 1572159600},
            {2, 1586073600},
            {3, 1603609200},
            {2, 1617523200},
            {3, 1635663600},
            {2, 1648972800},
            {3, 1667113200},
            {2, 1680422400},
            {3, 1698562800},
            {2, 1712476800},
            {3, 1730012400},
            {2, 1743926400},
            {3, 1761462000},
            {2, 1775376000},
            {3, 1792911600},
            {2, 1806825600},
            {3, 1824966000},
            {2, 1838275200},
            {3, 1856415600},
            {2, 1869724800},
            {3, 1887865200},
            {2, 1901779200},
            {3, 1919314800},
            {2, 1933228800},
            {3, 1950764400},
            {2, 1964678400},
            {3, 1982818800},
            {2, 1996128000},
            {3, 2014268400},
            {2, 2027577600},
            {3, 2045718000},
            {2, 2059027200},
            {3, 2077167600},
            {2, 2091081600},
            {3, 2108617200},
            {2, 2122531200},
            {3, 2140066800},
        }};
    }

    inline constexpr db::entry monterrey{"America/Monterrey",
                                         "Central Time - Durango; Coahuila, Nuevo León, Tamaulipas (most areas)",
                                         "+2540-10019",
                                         detail::monterrey::country_codes,
                                         detail::monterrey::offsets,
                                         detail::monterrey::transitions};

    // clang-format on
}
