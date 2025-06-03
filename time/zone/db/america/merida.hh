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

        inline constexpr array<zone::transition, 57> transitions{{
            {1, -1514743200},
            {2, 378201600},
            {4, 405068400},
            {3, 828864000},
            {4, 846399600},
            {3, 860313600},
            {4, 877849200},
            {3, 891763200},
            {4, 909298800},
            {3, 923212800},
            {4, 941353200},
            {3, 954662400},
            {4, 972802800},
            {3, 989136000},
            {4, 1001833200},
            {3, 1018166400},
            {4, 1035702000},
            {3, 1049616000},
            {4, 1067151600},
            {3, 1081065600},
            {4, 1099206000},
            {3, 1112515200},
            {4, 1130655600},
            {3, 1143964800},
            {4, 1162105200},
            {3, 1175414400},
            {4, 1193554800},
            {3, 1207468800},
            {4, 1225004400},
            {3, 1238918400},
            {4, 1256454000},
            {3, 1270368000},
            {4, 1288508400},
            {3, 1301817600},
            {4, 1319958000},
            {3, 1333267200},
            {4, 1351407600},
            {3, 1365321600},
            {4, 1382857200},
            {3, 1396771200},
            {4, 1414306800},
            {3, 1428220800},
            {4, 1445756400},
            {3, 1459670400},
            {4, 1477810800},
            {3, 1491120000},
            {4, 1509260400},
            {3, 1522569600},
            {4, 1540710000},
            {3, 1554624000},
            {4, 1572159600},
            {3, 1586073600},
            {4, 1603609200},
            {3, 1617523200},
            {4, 1635663600},
            {3, 1648972800},
            {4, 1667113200},
        }};
    }

    inline constexpr db::entry merida{"America/Merida",
                                      "Campeche, Yucatán",
                                      "+2058-08937",
                                      detail::merida::country_codes,
                                      detail::merida::offsets,
                                      detail::merida::transitions};

    // clang-format on
}
