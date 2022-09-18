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

        inline constexpr array<zone::offset, 3> offsets{{
            {"LMT", -24076, false},
            {"CST", -21600, false},
            {"CDT", -18000, true},
        }};

        inline constexpr array<zone::transition, 87> transitions{{
            {1, -1514743200},
            {2, 576057600},
            {1, 594198000},
            {2, 828864000},
            {1, 846399600},
            {2, 860313600},
            {1, 877849200},
            {2, 891763200},
            {1, 909298800},
            {2, 923212800},
            {1, 941353200},
            {2, 954662400},
            {1, 972802800},
            {2, 989136000},
            {1, 1001833200},
            {2, 1018166400},
            {1, 1035702000},
            {2, 1049616000},
            {1, 1067151600},
            {2, 1081065600},
            {1, 1099206000},
            {2, 1112515200},
            {1, 1130655600},
            {2, 1143964800},
            {1, 1162105200},
            {2, 1175414400},
            {1, 1193554800},
            {2, 1207468800},
            {1, 1225004400},
            {2, 1238918400},
            {1, 1256454000},
            {2, 1270368000},
            {1, 1288508400},
            {2, 1301817600},
            {1, 1319958000},
            {2, 1333267200},
            {1, 1351407600},
            {2, 1365321600},
            {1, 1382857200},
            {2, 1396771200},
            {1, 1414306800},
            {2, 1428220800},
            {1, 1445756400},
            {2, 1459670400},
            {1, 1477810800},
            {2, 1491120000},
            {1, 1509260400},
            {2, 1522569600},
            {1, 1540710000},
            {2, 1554624000},
            {1, 1572159600},
            {2, 1586073600},
            {1, 1603609200},
            {2, 1617523200},
            {1, 1635663600},
            {2, 1648972800},
            {1, 1667113200},
            {2, 1680422400},
            {1, 1698562800},
            {2, 1712476800},
            {1, 1730012400},
            {2, 1743926400},
            {1, 1761462000},
            {2, 1775376000},
            {1, 1792911600},
            {2, 1806825600},
            {1, 1824966000},
            {2, 1838275200},
            {1, 1856415600},
            {2, 1869724800},
            {1, 1887865200},
            {2, 1901779200},
            {1, 1919314800},
            {2, 1933228800},
            {1, 1950764400},
            {2, 1964678400},
            {1, 1982818800},
            {2, 1996128000},
            {1, 2014268400},
            {2, 2027577600},
            {1, 2045718000},
            {2, 2059027200},
            {1, 2077167600},
            {2, 2091081600},
            {1, 2108617200},
            {2, 2122531200},
            {1, 2140066800},
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
