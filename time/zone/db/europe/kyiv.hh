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

namespace snn::time::zone::db::europe
{
    // clang-format off

    namespace detail::kyiv
    {
        inline constexpr array<country::code, 1> country_codes{
            "UA",
        };

        inline constexpr array<zone::offset, 15> offsets{{
            {"LMT", 7324, false},
            {"KMT", 7324, false},
            {"EET", 7200, false},
            {"MSK", 10800, false},
            {"CET", 3600, false},
            {"CEST", 7200, true},
            {"CEST", 7200, true},
            {"MSD", 14400, true},
            {"MSK", 10800, false},
            {"MSD", 14400, true},
            {"EEST", 10800, true},
            {"EET", 7200, false},
            {"EEST", 10800, true},
            {"EET", 7200, false},
            {"EEST", 10800, true},
        }};

        inline constexpr array<zone::transition, 121> transitions{{
            {1, -2840148124},
            {2, -1441159324},
            {3, -1247536800},
            {6, -892522800},
            {4, -857257200},
            {5, -844556400},
            {4, -828226800},
            {3, -825382800},
            {7, 354920400},
            {3, 370728000},
            {7, 386456400},
            {3, 402264000},
            {7, 417992400},
            {3, 433800000},
            {7, 449614800},
            {8, 465346800},
            {9, 481071600},
            {8, 496796400},
            {9, 512521200},
            {8, 528246000},
            {9, 543970800},
            {8, 559695600},
            {9, 575420400},
            {8, 591145200},
            {9, 606870000},
            {8, 622594800},
            {9, 638319600},
            {10, 646783200},
            {11, 686102400},
            {12, 701827200},
            {11, 717552000},
            {12, 733276800},
            {11, 749001600},
            {12, 764726400},
            {11, 780451200},
            {12, 796176000},
            {11, 811900800},
            {12, 828230400},
            {13, 846378000},
            {14, 859683600},
            {13, 877827600},
            {14, 891133200},
            {13, 909277200},
            {14, 922582800},
            {13, 941331600},
            {14, 954032400},
            {13, 972781200},
            {14, 985482000},
            {13, 1004230800},
            {14, 1017536400},
            {13, 1035680400},
            {14, 1048986000},
            {13, 1067130000},
            {14, 1080435600},
            {13, 1099184400},
            {14, 1111885200},
            {13, 1130634000},
            {14, 1143334800},
            {13, 1162083600},
            {14, 1174784400},
            {13, 1193533200},
            {14, 1206838800},
            {13, 1224982800},
            {14, 1238288400},
            {13, 1256432400},
            {14, 1269738000},
            {13, 1288486800},
            {14, 1301187600},
            {13, 1319936400},
            {14, 1332637200},
            {13, 1351386000},
            {14, 1364691600},
            {13, 1382835600},
            {14, 1396141200},
            {13, 1414285200},
            {14, 1427590800},
            {13, 1445734800},
            {14, 1459040400},
            {13, 1477789200},
            {14, 1490490000},
            {13, 1509238800},
            {14, 1521939600},
            {13, 1540688400},
            {14, 1553994000},
            {13, 1572138000},
            {14, 1585443600},
            {13, 1603587600},
            {14, 1616893200},
            {13, 1635642000},
            {14, 1648342800},
            {13, 1667091600},
            {14, 1679792400},
            {13, 1698541200},
            {14, 1711846800},
            {13, 1729990800},
            {14, 1743296400},
            {13, 1761440400},
            {14, 1774746000},
            {13, 1792890000},
            {14, 1806195600},
            {13, 1824944400},
            {14, 1837645200},
            {13, 1856394000},
            {14, 1869094800},
            {13, 1887843600},
            {14, 1901149200},
            {13, 1919293200},
            {14, 1932598800},
            {13, 1950742800},
            {14, 1964048400},
            {13, 1982797200},
            {14, 1995498000},
            {13, 2014246800},
            {14, 2026947600},
            {13, 2045696400},
            {14, 2058397200},
            {13, 2077146000},
            {14, 2090451600},
            {13, 2108595600},
            {14, 2121901200},
            {13, 2140045200},
        }};
    }

    inline constexpr db::entry kyiv{"Europe/Kyiv",
                                    "Ukraine (most areas)",
                                    "+5026+03031",
                                    detail::kyiv::country_codes,
                                    detail::kyiv::offsets,
                                    detail::kyiv::transitions};

    // clang-format on
}
