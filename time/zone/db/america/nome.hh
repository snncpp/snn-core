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

    namespace detail::nome
    {
        inline constexpr array<country::code, 1> country_codes{
            "US",
        };

        inline constexpr array<zone::offset, 10> offsets{{
            {"LMT", 46702, false},
            {"LMT", -39698, false},
            {"NST", -39600, false},
            {"NWT", -36000, true},
            {"NPT", -36000, true},
            {"BST", -39600, false},
            {"BDT", -36000, true},
            {"YST", -32400, false},
            {"AKDT", -28800, true},
            {"AKST", -32400, false},
        }};

        inline constexpr array<zone::transition, 145> transitions{{
            {1, -3225223727},
            {2, -2188947502},
            {3, -880196400},
            {4, -769395600},
            {2, -765374400},
            {5, -86878800},
            {6, -21466800},
            {5, -5745600},
            {6, 9982800},
            {5, 25704000},
            {6, 41432400},
            {5, 57758400},
            {6, 73486800},
            {5, 89208000},
            {6, 104936400},
            {5, 120657600},
            {6, 126709200},
            {5, 152107200},
            {6, 162392400},
            {5, 183556800},
            {6, 199285200},
            {5, 215611200},
            {6, 230734800},
            {5, 247060800},
            {6, 262789200},
            {5, 278510400},
            {6, 294238800},
            {5, 309960000},
            {6, 325688400},
            {5, 341409600},
            {6, 357138000},
            {5, 372859200},
            {6, 388587600},
            {5, 404913600},
            {6, 420037200},
            {7, 436363200},
            {9, 439030800},
            {8, 452084400},
            {9, 467805600},
            {8, 483534000},
            {9, 499255200},
            {8, 514983600},
            {9, 530704800},
            {8, 544618800},
            {9, 562154400},
            {8, 576068400},
            {9, 594208800},
            {8, 607518000},
            {9, 625658400},
            {8, 638967600},
            {9, 657108000},
            {8, 671022000},
            {9, 688557600},
            {8, 702471600},
            {9, 720007200},
            {8, 733921200},
            {9, 752061600},
            {8, 765370800},
            {9, 783511200},
            {8, 796820400},
            {9, 814960800},
            {8, 828874800},
            {9, 846410400},
            {8, 860324400},
            {9, 877860000},
            {8, 891774000},
            {9, 909309600},
            {8, 923223600},
            {9, 941364000},
            {8, 954673200},
            {9, 972813600},
            {8, 986122800},
            {9, 1004263200},
            {8, 1018177200},
            {9, 1035712800},
            {8, 1049626800},
            {9, 1067162400},
            {8, 1081076400},
            {9, 1099216800},
            {8, 1112526000},
            {9, 1130666400},
            {8, 1143975600},
            {9, 1162116000},
            {8, 1173610800},
            {9, 1194170400},
            {8, 1205060400},
            {9, 1225620000},
            {8, 1236510000},
            {9, 1257069600},
            {8, 1268564400},
            {9, 1289124000},
            {8, 1300014000},
            {9, 1320573600},
            {8, 1331463600},
            {9, 1352023200},
            {8, 1362913200},
            {9, 1383472800},
            {8, 1394362800},
            {9, 1414922400},
            {8, 1425812400},
            {9, 1446372000},
            {8, 1457866800},
            {9, 1478426400},
            {8, 1489316400},
            {9, 1509876000},
            {8, 1520766000},
            {9, 1541325600},
            {8, 1552215600},
            {9, 1572775200},
            {8, 1583665200},
            {9, 1604224800},
            {8, 1615719600},
            {9, 1636279200},
            {8, 1647169200},
            {9, 1667728800},
            {8, 1678618800},
            {9, 1699178400},
            {8, 1710068400},
            {9, 1730628000},
            {8, 1741518000},
            {9, 1762077600},
            {8, 1772967600},
            {9, 1793527200},
            {8, 1805022000},
            {9, 1825581600},
            {8, 1836471600},
            {9, 1857031200},
            {8, 1867921200},
            {9, 1888480800},
            {8, 1899370800},
            {9, 1919930400},
            {8, 1930820400},
            {9, 1951380000},
            {8, 1962874800},
            {9, 1983434400},
            {8, 1994324400},
            {9, 2014884000},
            {8, 2025774000},
            {9, 2046333600},
            {8, 2057223600},
            {9, 2077783200},
            {8, 2088673200},
            {9, 2109232800},
            {8, 2120122800},
            {9, 2140682400},
        }};
    }

    inline constexpr db::entry nome{"America/Nome",
                                    "Alaska (west)",
                                    "+643004-1652423",
                                    detail::nome::country_codes,
                                    detail::nome::offsets,
                                    detail::nome::transitions};

    // clang-format on
}
