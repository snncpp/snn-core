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

        inline constexpr array<zone::offset, 7> offsets{{
            {"LMT", -24076, false},
            {"MST", -25200, false},
            {"CST", -21600, false},
            {"MDT", -21600, true},
            {"MST", -25200, false},
            {"CDT", -18000, true},
            {"CST", -21600, false},
        }};

        inline constexpr array<zone::transition, 62> transitions{{
            {1, -1514743200},
            {2, -1343149200},
            {4, -1234807200},
            {3, -1220461200},
            {4, -1207159200},
            {2, -1191344400},
            {5, 576057600},
            {2, 594198000},
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

    inline constexpr db::entry monterrey{"America/Monterrey",
                                         "Durango; Coahuila, Nuevo León, Tamaulipas (most areas)",
                                         "+2540-10019",
                                         detail::monterrey::country_codes,
                                         detail::monterrey::offsets,
                                         detail::monterrey::transitions};

    // clang-format on
}
