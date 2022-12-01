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

    namespace detail::matamoros
    {
        inline constexpr array<country::code, 1> country_codes{
            "MX",
        };

        inline constexpr array<zone::offset, 4> offsets{{
            {"LMT", -23400, false},
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
            {2, 1268553600},
            {3, 1289113200},
            {2, 1300003200},
            {3, 1320562800},
            {2, 1331452800},
            {3, 1352012400},
            {2, 1362902400},
            {3, 1383462000},
            {2, 1394352000},
            {3, 1414911600},
            {2, 1425801600},
            {3, 1446361200},
            {2, 1457856000},
            {3, 1478415600},
            {2, 1489305600},
            {3, 1509865200},
            {2, 1520755200},
            {3, 1541314800},
            {2, 1552204800},
            {3, 1572764400},
            {2, 1583654400},
            {3, 1604214000},
            {2, 1615708800},
            {3, 1636268400},
            {2, 1647158400},
            {3, 1667718000},
            {2, 1678608000},
            {3, 1699167600},
            {2, 1710057600},
            {3, 1730617200},
            {2, 1741507200},
            {3, 1762066800},
            {2, 1772956800},
            {3, 1793516400},
            {2, 1805011200},
            {3, 1825570800},
            {2, 1836460800},
            {3, 1857020400},
            {2, 1867910400},
            {3, 1888470000},
            {2, 1899360000},
            {3, 1919919600},
            {2, 1930809600},
            {3, 1951369200},
            {2, 1962864000},
            {3, 1983423600},
            {2, 1994313600},
            {3, 2014873200},
            {2, 2025763200},
            {3, 2046322800},
            {2, 2057212800},
            {3, 2077772400},
            {2, 2088662400},
            {3, 2109222000},
            {2, 2120112000},
            {3, 2140671600},
        }};
    }

    inline constexpr db::entry matamoros{"America/Matamoros",
                                         "Coahuila, Nuevo León, Tamaulipas (US border)",
                                         "+2550-09730",
                                         detail::matamoros::country_codes,
                                         detail::matamoros::offsets,
                                         detail::matamoros::transitions};

    // clang-format on
}
