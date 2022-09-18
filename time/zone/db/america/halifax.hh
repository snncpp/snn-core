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

    namespace detail::halifax
    {
        inline constexpr array<country::code, 1> country_codes{
            "CA",
        };

        inline constexpr array<zone::offset, 5> offsets{{
            {"LMT", -15264, false},
            {"ADT", -10800, true},
            {"AST", -14400, false},
            {"AWT", -10800, true},
            {"APT", -10800, true},
        }};

        inline constexpr array<zone::transition, 228> transitions{{
            {2, -2131645536},
            {1, -1696276800},
            {2, -1680469200},
            {1, -1632074400},
            {2, -1615143600},
            {1, -1566763200},
            {2, -1557090000},
            {1, -1535486400},
            {2, -1524949200},
            {1, -1504468800},
            {2, -1493413200},
            {1, -1472414400},
            {2, -1461963600},
            {1, -1440964800},
            {2, -1429390800},
            {1, -1409515200},
            {2, -1396731600},
            {1, -1376856000},
            {2, -1366491600},
            {1, -1346616000},
            {2, -1333832400},
            {1, -1313956800},
            {2, -1303678800},
            {1, -1282507200},
            {2, -1272661200},
            {1, -1251057600},
            {2, -1240088400},
            {1, -1219608000},
            {2, -1207429200},
            {1, -1188763200},
            {2, -1175979600},
            {1, -1157313600},
            {2, -1143925200},
            {1, -1124049600},
            {2, -1113771600},
            {1, -1091390400},
            {2, -1081026000},
            {1, -1059854400},
            {2, -1050786000},
            {1, -1030910400},
            {2, -1018126800},
            {1, -999460800},
            {2, -986677200},
            {1, -965592000},
            {2, -955227600},
            {1, -935956800},
            {2, -923173200},
            {1, -904507200},
            {2, -891723600},
            {3, -880221600},
            {4, -769395600},
            {2, -765399600},
            {1, -747252000},
            {2, -733950000},
            {1, -715802400},
            {2, -702500400},
            {1, -684352800},
            {2, -671050800},
            {1, -652903200},
            {2, -639601200},
            {1, -589399200},
            {2, -576097200},
            {1, -557949600},
            {2, -544647600},
            {1, -526500000},
            {2, -513198000},
            {1, -495050400},
            {2, -481748400},
            {1, -431546400},
            {2, -418244400},
            {1, -400096800},
            {2, -386794800},
            {1, -368647200},
            {2, -355345200},
            {1, -337197600},
            {2, -323895600},
            {1, -242244000},
            {2, -226522800},
            {1, -210794400},
            {2, -195073200},
            {1, -179344800},
            {2, -163623600},
            {1, -147895200},
            {2, -131569200},
            {1, -116445600},
            {2, -100119600},
            {1, -84391200},
            {2, -68670000},
            {1, -52941600},
            {2, -37220400},
            {1, -21492000},
            {2, -5770800},
            {1, 9957600},
            {2, 25678800},
            {1, 41407200},
            {2, 57733200},
            {1, 73461600},
            {2, 89182800},
            {1, 104911200},
            {2, 120632400},
            {1, 136360800},
            {2, 152082000},
            {1, 167810400},
            {2, 183531600},
            {1, 199260000},
            {2, 215586000},
            {1, 230709600},
            {2, 247035600},
            {1, 262764000},
            {2, 278485200},
            {1, 294213600},
            {2, 309934800},
            {1, 325663200},
            {2, 341384400},
            {1, 357112800},
            {2, 372834000},
            {1, 388562400},
            {2, 404888400},
            {1, 420012000},
            {2, 436338000},
            {1, 452066400},
            {2, 467787600},
            {1, 483516000},
            {2, 499237200},
            {1, 514965600},
            {2, 530686800},
            {1, 544600800},
            {2, 562136400},
            {1, 576050400},
            {2, 594190800},
            {1, 607500000},
            {2, 625640400},
            {1, 638949600},
            {2, 657090000},
            {1, 671004000},
            {2, 688539600},
            {1, 702453600},
            {2, 719989200},
            {1, 733903200},
            {2, 752043600},
            {1, 765352800},
            {2, 783493200},
            {1, 796802400},
            {2, 814942800},
            {1, 828856800},
            {2, 846392400},
            {1, 860306400},
            {2, 877842000},
            {1, 891756000},
            {2, 909291600},
            {1, 923205600},
            {2, 941346000},
            {1, 954655200},
            {2, 972795600},
            {1, 986104800},
            {2, 1004245200},
            {1, 1018159200},
            {2, 1035694800},
            {1, 1049608800},
            {2, 1067144400},
            {1, 1081058400},
            {2, 1099198800},
            {1, 1112508000},
            {2, 1130648400},
            {1, 1143957600},
            {2, 1162098000},
            {1, 1173592800},
            {2, 1194152400},
            {1, 1205042400},
            {2, 1225602000},
            {1, 1236492000},
            {2, 1257051600},
            {1, 1268546400},
            {2, 1289106000},
            {1, 1299996000},
            {2, 1320555600},
            {1, 1331445600},
            {2, 1352005200},
            {1, 1362895200},
            {2, 1383454800},
            {1, 1394344800},
            {2, 1414904400},
            {1, 1425794400},
            {2, 1446354000},
            {1, 1457848800},
            {2, 1478408400},
            {1, 1489298400},
            {2, 1509858000},
            {1, 1520748000},
            {2, 1541307600},
            {1, 1552197600},
            {2, 1572757200},
            {1, 1583647200},
            {2, 1604206800},
            {1, 1615701600},
            {2, 1636261200},
            {1, 1647151200},
            {2, 1667710800},
            {1, 1678600800},
            {2, 1699160400},
            {1, 1710050400},
            {2, 1730610000},
            {1, 1741500000},
            {2, 1762059600},
            {1, 1772949600},
            {2, 1793509200},
            {1, 1805004000},
            {2, 1825563600},
            {1, 1836453600},
            {2, 1857013200},
            {1, 1867903200},
            {2, 1888462800},
            {1, 1899352800},
            {2, 1919912400},
            {1, 1930802400},
            {2, 1951362000},
            {1, 1962856800},
            {2, 1983416400},
            {1, 1994306400},
            {2, 2014866000},
            {1, 2025756000},
            {2, 2046315600},
            {1, 2057205600},
            {2, 2077765200},
            {1, 2088655200},
            {2, 2109214800},
            {1, 2120104800},
            {2, 2140664400},
        }};
    }

    inline constexpr db::entry halifax{"America/Halifax",
                                       "Atlantic - NS (most areas); PE",
                                       "+4439-06336",
                                       detail::halifax::country_codes,
                                       detail::halifax::offsets,
                                       detail::halifax::transitions};

    // clang-format on
}
