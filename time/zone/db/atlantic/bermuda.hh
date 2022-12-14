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

namespace snn::time::zone::db::atlantic
{
    // clang-format off

    namespace detail::bermuda
    {
        inline constexpr array<country::code, 1> country_codes{
            "BM",
        };

        inline constexpr array<zone::offset, 5> offsets{{
            {"LMT", -15558, false},
            {"BST", -11958, true},
            {"BMT", -15558, false},
            {"ADT", -10800, true},
            {"AST", -14400, false},
        }};

        inline constexpr array<zone::transition, 156> transitions{{
            {2, -2524506042},
            {1, -1664307642},
            {2, -1648932042},
            {1, -1632080442},
            {2, -1618692042},
            {4, -1262281242},
            {3, -882727200},
            {4, -858538800},
            {3, -845229600},
            {4, -825879600},
            {3, -814384800},
            {4, -793825200},
            {3, -782935200},
            {4, -762375600},
            {3, -713988000},
            {4, -703710000},
            {3, -681933600},
            {4, -672865200},
            {3, -650484000},
            {4, -641415600},
            {3, -618429600},
            {4, -609966000},
            {3, -586980000},
            {4, -578516400},
            {3, -555530400},
            {4, -546462000},
            {3, -429127200},
            {4, -415825200},
            {3, 136360800},
            {4, 152082000},
            {3, 167810400},
            {4, 183531600},
            {3, 199260000},
            {4, 215586000},
            {3, 230709600},
            {4, 247035600},
            {3, 262764000},
            {4, 278485200},
            {3, 294213600},
            {4, 309934800},
            {3, 325663200},
            {4, 341384400},
            {3, 357112800},
            {4, 372834000},
            {3, 388562400},
            {4, 404888400},
            {3, 420012000},
            {4, 436338000},
            {3, 452066400},
            {4, 467787600},
            {3, 483516000},
            {4, 499237200},
            {3, 514965600},
            {4, 530686800},
            {3, 544600800},
            {4, 562136400},
            {3, 576050400},
            {4, 594190800},
            {3, 607500000},
            {4, 625640400},
            {3, 638949600},
            {4, 657090000},
            {3, 671004000},
            {4, 688539600},
            {3, 702453600},
            {4, 719989200},
            {3, 733903200},
            {4, 752043600},
            {3, 765352800},
            {4, 783493200},
            {3, 796802400},
            {4, 814942800},
            {3, 828856800},
            {4, 846392400},
            {3, 860306400},
            {4, 877842000},
            {3, 891756000},
            {4, 909291600},
            {3, 923205600},
            {4, 941346000},
            {3, 954655200},
            {4, 972795600},
            {3, 986104800},
            {4, 1004245200},
            {3, 1018159200},
            {4, 1035694800},
            {3, 1049608800},
            {4, 1067144400},
            {3, 1081058400},
            {4, 1099198800},
            {3, 1112508000},
            {4, 1130648400},
            {3, 1143957600},
            {4, 1162098000},
            {3, 1173592800},
            {4, 1194152400},
            {3, 1205042400},
            {4, 1225602000},
            {3, 1236492000},
            {4, 1257051600},
            {3, 1268546400},
            {4, 1289106000},
            {3, 1299996000},
            {4, 1320555600},
            {3, 1331445600},
            {4, 1352005200},
            {3, 1362895200},
            {4, 1383454800},
            {3, 1394344800},
            {4, 1414904400},
            {3, 1425794400},
            {4, 1446354000},
            {3, 1457848800},
            {4, 1478408400},
            {3, 1489298400},
            {4, 1509858000},
            {3, 1520748000},
            {4, 1541307600},
            {3, 1552197600},
            {4, 1572757200},
            {3, 1583647200},
            {4, 1604206800},
            {3, 1615701600},
            {4, 1636261200},
            {3, 1647151200},
            {4, 1667710800},
            {3, 1678600800},
            {4, 1699160400},
            {3, 1710050400},
            {4, 1730610000},
            {3, 1741500000},
            {4, 1762059600},
            {3, 1772949600},
            {4, 1793509200},
            {3, 1805004000},
            {4, 1825563600},
            {3, 1836453600},
            {4, 1857013200},
            {3, 1867903200},
            {4, 1888462800},
            {3, 1899352800},
            {4, 1919912400},
            {3, 1930802400},
            {4, 1951362000},
            {3, 1962856800},
            {4, 1983416400},
            {3, 1994306400},
            {4, 2014866000},
            {3, 2025756000},
            {4, 2046315600},
            {3, 2057205600},
            {4, 2077765200},
            {3, 2088655200},
            {4, 2109214800},
            {3, 2120104800},
            {4, 2140664400},
        }};
    }

    inline constexpr db::entry bermuda{"Atlantic/Bermuda",
                                       "",
                                       "+3217-06446",
                                       detail::bermuda::country_codes,
                                       detail::bermuda::offsets,
                                       detail::bermuda::transitions};

    // clang-format on
}
