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

    namespace detail::athens
    {
        inline constexpr array<country::code, 1> country_codes{
            "GR",
        };

        inline constexpr array<zone::offset, 10> offsets{{
            {"LMT", 5692, false},
            {"AMT", 5692, false},
            {"EEST", 10800, true},
            {"EET", 7200, false},
            {"CET", 3600, false},
            {"CEST", 7200, true},
            {"EEST", 10800, true},
            {"EET", 7200, false},
            {"EEST", 10800, true},
            {"EET", 7200, false},
        }};

        inline constexpr array<zone::transition, 138> transitions{{
            {1, -2344642492},
            {3, -1686101632},
            {2, -1182996000},
            {3, -1178161200},
            {2, -906861600},
            {5, -904878000},
            {4, -857257200},
            {5, -844477200},
            {4, -828237600},
            {3, -812422800},
            {2, -552362400},
            {3, -541652400},
            {6, 166485600},
            {7, 186184800},
            {6, 198028800},
            {7, 213753600},
            {6, 228873600},
            {7, 244080000},
            {6, 260323200},
            {3, 275446800},
            {2, 291798000},
            {3, 307407600},
            {2, 323388000},
            {3, 338936400},
            {8, 354675600},
            {9, 370400400},
            {8, 386125200},
            {9, 401850000},
            {8, 417574800},
            {9, 433299600},
            {8, 449024400},
            {9, 465354000},
            {8, 481078800},
            {9, 496803600},
            {8, 512528400},
            {9, 528253200},
            {8, 543978000},
            {9, 559702800},
            {8, 575427600},
            {9, 591152400},
            {8, 606877200},
            {9, 622602000},
            {8, 638326800},
            {9, 654656400},
            {8, 670381200},
            {9, 686106000},
            {8, 701830800},
            {9, 717555600},
            {8, 733280400},
            {9, 749005200},
            {8, 764730000},
            {9, 780454800},
            {8, 796179600},
            {9, 811904400},
            {8, 828234000},
            {9, 846378000},
            {8, 859683600},
            {9, 877827600},
            {8, 891133200},
            {9, 909277200},
            {8, 922582800},
            {9, 941331600},
            {8, 954032400},
            {9, 972781200},
            {8, 985482000},
            {9, 1004230800},
            {8, 1017536400},
            {9, 1035680400},
            {8, 1048986000},
            {9, 1067130000},
            {8, 1080435600},
            {9, 1099184400},
            {8, 1111885200},
            {9, 1130634000},
            {8, 1143334800},
            {9, 1162083600},
            {8, 1174784400},
            {9, 1193533200},
            {8, 1206838800},
            {9, 1224982800},
            {8, 1238288400},
            {9, 1256432400},
            {8, 1269738000},
            {9, 1288486800},
            {8, 1301187600},
            {9, 1319936400},
            {8, 1332637200},
            {9, 1351386000},
            {8, 1364691600},
            {9, 1382835600},
            {8, 1396141200},
            {9, 1414285200},
            {8, 1427590800},
            {9, 1445734800},
            {8, 1459040400},
            {9, 1477789200},
            {8, 1490490000},
            {9, 1509238800},
            {8, 1521939600},
            {9, 1540688400},
            {8, 1553994000},
            {9, 1572138000},
            {8, 1585443600},
            {9, 1603587600},
            {8, 1616893200},
            {9, 1635642000},
            {8, 1648342800},
            {9, 1667091600},
            {8, 1679792400},
            {9, 1698541200},
            {8, 1711846800},
            {9, 1729990800},
            {8, 1743296400},
            {9, 1761440400},
            {8, 1774746000},
            {9, 1792890000},
            {8, 1806195600},
            {9, 1824944400},
            {8, 1837645200},
            {9, 1856394000},
            {8, 1869094800},
            {9, 1887843600},
            {8, 1901149200},
            {9, 1919293200},
            {8, 1932598800},
            {9, 1950742800},
            {8, 1964048400},
            {9, 1982797200},
            {8, 1995498000},
            {9, 2014246800},
            {8, 2026947600},
            {9, 2045696400},
            {8, 2058397200},
            {9, 2077146000},
            {8, 2090451600},
            {9, 2108595600},
            {8, 2121901200},
            {9, 2140045200},
        }};
    }

    inline constexpr db::entry athens{"Europe/Athens",
                                      "",
                                      "+3758+02343",
                                      detail::athens::country_codes,
                                      detail::athens::offsets,
                                      detail::athens::transitions};

    // clang-format on
}
