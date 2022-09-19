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

namespace snn::time::zone::db::america::indiana
{
    // clang-format off

    namespace detail::tell_city
    {
        inline constexpr array<country::code, 1> country_codes{
            "US",
        };

        inline constexpr array<zone::offset, 9> offsets{{
            {"LMT", -20823, false},
            {"CDT", -18000, true},
            {"CST", -21600, false},
            {"CWT", -18000, true},
            {"CPT", -18000, true},
            {"EST", -18000, false},
            {"EDT", -14400, true},
            {"CDT", -18000, true},
            {"CST", -21600, false},
        }};

        inline constexpr array<zone::transition, 98> transitions{{
            {2, -2717647200},
            {1, -1633276800},
            {2, -1615136400},
            {1, -1601827200},
            {2, -1583686800},
            {3, -880214400},
            {4, -769395600},
            {2, -765392400},
            {1, -462996000},
            {2, -450291600},
            {1, -431539200},
            {2, -418237200},
            {1, -400089600},
            {2, -386787600},
            {1, -368640000},
            {2, -355338000},
            {1, -337190400},
            {2, -323888400},
            {1, -305740800},
            {2, -292438800},
            {1, -273686400},
            {2, -257965200},
            {1, -242236800},
            {2, -226515600},
            {1, -210787200},
            {2, -195066000},
            {5, -179337600},
            {2, -68662800},
            {1, -52934400},
            {2, -37213200},
            {6, -21484800},
            {5, -5767200},
            {6, 9961200},
            {5, 25682400},
            {1, 1143961200},
            {2, 1162105200},
            {1, 1173600000},
            {2, 1194159600},
            {1, 1205049600},
            {2, 1225609200},
            {1, 1236499200},
            {2, 1257058800},
            {1, 1268553600},
            {2, 1289113200},
            {1, 1300003200},
            {2, 1320562800},
            {1, 1331452800},
            {2, 1352012400},
            {1, 1362902400},
            {2, 1383462000},
            {1, 1394352000},
            {2, 1414911600},
            {1, 1425801600},
            {2, 1446361200},
            {1, 1457856000},
            {2, 1478415600},
            {1, 1489305600},
            {2, 1509865200},
            {1, 1520755200},
            {2, 1541314800},
            {1, 1552204800},
            {2, 1572764400},
            {1, 1583654400},
            {2, 1604214000},
            {1, 1615708800},
            {2, 1636268400},
            {1, 1647158400},
            {2, 1667718000},
            {1, 1678608000},
            {2, 1699167600},
            {1, 1710057600},
            {2, 1730617200},
            {1, 1741507200},
            {2, 1762066800},
            {1, 1772956800},
            {2, 1793516400},
            {1, 1805011200},
            {2, 1825570800},
            {1, 1836460800},
            {2, 1857020400},
            {1, 1867910400},
            {2, 1888470000},
            {1, 1899360000},
            {2, 1919919600},
            {1, 1930809600},
            {2, 1951369200},
            {1, 1962864000},
            {2, 1983423600},
            {1, 1994313600},
            {2, 2014873200},
            {1, 2025763200},
            {2, 2046322800},
            {1, 2057212800},
            {2, 2077772400},
            {1, 2088662400},
            {2, 2109222000},
            {1, 2120112000},
            {2, 2140671600},
        }};
    }

    inline constexpr db::entry tell_city{"America/Indiana/Tell_City",
                                         "Central - IN (Perry)",
                                         "+375711-0864541",
                                         detail::tell_city::country_codes,
                                         detail::tell_city::offsets,
                                         detail::tell_city::transitions};

    // clang-format on
}