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

    namespace detail::yakutat
    {
        inline constexpr array<country::code, 1> country_codes{
            "US",
        };

        inline constexpr array<zone::offset, 8> offsets{{
            {"LMT", 52865, false},
            {"LMT", -33535, false},
            {"YST", -32400, false},
            {"YWT", -28800, true},
            {"YPT", -28800, true},
            {"YDT", -28800, true},
            {"AKDT", -28800, true},
            {"AKST", -32400, false},
        }};

        inline constexpr array<zone::transition, 144> transitions{{
            {1, -3225223727},
            {2, -2188953665},
            {3, -880203600},
            {4, -769395600},
            {2, -765381600},
            {5, -21474000},
            {2, -5752800},
            {5, 9975600},
            {2, 25696800},
            {5, 41425200},
            {2, 57751200},
            {5, 73479600},
            {2, 89200800},
            {5, 104929200},
            {2, 120650400},
            {5, 126702000},
            {2, 152100000},
            {5, 162385200},
            {2, 183549600},
            {5, 199278000},
            {2, 215604000},
            {5, 230727600},
            {2, 247053600},
            {5, 262782000},
            {2, 278503200},
            {5, 294231600},
            {2, 309952800},
            {5, 325681200},
            {2, 341402400},
            {5, 357130800},
            {2, 372852000},
            {5, 388580400},
            {2, 404906400},
            {5, 420030000},
            {2, 436356000},
            {7, 439030800},
            {6, 452084400},
            {7, 467805600},
            {6, 483534000},
            {7, 499255200},
            {6, 514983600},
            {7, 530704800},
            {6, 544618800},
            {7, 562154400},
            {6, 576068400},
            {7, 594208800},
            {6, 607518000},
            {7, 625658400},
            {6, 638967600},
            {7, 657108000},
            {6, 671022000},
            {7, 688557600},
            {6, 702471600},
            {7, 720007200},
            {6, 733921200},
            {7, 752061600},
            {6, 765370800},
            {7, 783511200},
            {6, 796820400},
            {7, 814960800},
            {6, 828874800},
            {7, 846410400},
            {6, 860324400},
            {7, 877860000},
            {6, 891774000},
            {7, 909309600},
            {6, 923223600},
            {7, 941364000},
            {6, 954673200},
            {7, 972813600},
            {6, 986122800},
            {7, 1004263200},
            {6, 1018177200},
            {7, 1035712800},
            {6, 1049626800},
            {7, 1067162400},
            {6, 1081076400},
            {7, 1099216800},
            {6, 1112526000},
            {7, 1130666400},
            {6, 1143975600},
            {7, 1162116000},
            {6, 1173610800},
            {7, 1194170400},
            {6, 1205060400},
            {7, 1225620000},
            {6, 1236510000},
            {7, 1257069600},
            {6, 1268564400},
            {7, 1289124000},
            {6, 1300014000},
            {7, 1320573600},
            {6, 1331463600},
            {7, 1352023200},
            {6, 1362913200},
            {7, 1383472800},
            {6, 1394362800},
            {7, 1414922400},
            {6, 1425812400},
            {7, 1446372000},
            {6, 1457866800},
            {7, 1478426400},
            {6, 1489316400},
            {7, 1509876000},
            {6, 1520766000},
            {7, 1541325600},
            {6, 1552215600},
            {7, 1572775200},
            {6, 1583665200},
            {7, 1604224800},
            {6, 1615719600},
            {7, 1636279200},
            {6, 1647169200},
            {7, 1667728800},
            {6, 1678618800},
            {7, 1699178400},
            {6, 1710068400},
            {7, 1730628000},
            {6, 1741518000},
            {7, 1762077600},
            {6, 1772967600},
            {7, 1793527200},
            {6, 1805022000},
            {7, 1825581600},
            {6, 1836471600},
            {7, 1857031200},
            {6, 1867921200},
            {7, 1888480800},
            {6, 1899370800},
            {7, 1919930400},
            {6, 1930820400},
            {7, 1951380000},
            {6, 1962874800},
            {7, 1983434400},
            {6, 1994324400},
            {7, 2014884000},
            {6, 2025774000},
            {7, 2046333600},
            {6, 2057223600},
            {7, 2077783200},
            {6, 2088673200},
            {7, 2109232800},
            {6, 2120122800},
            {7, 2140682400},
        }};
    }

    inline constexpr db::entry yakutat{"America/Yakutat",
                                       "Alaska - Yakutat",
                                       "+593249-1394338",
                                       detail::yakutat::country_codes,
                                       detail::yakutat::offsets,
                                       detail::yakutat::transitions};

    // clang-format on
}