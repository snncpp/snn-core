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

    namespace detail::sofia
    {
        inline constexpr array<country::code, 1> country_codes{
            "BG",
        };

        inline constexpr array<zone::offset, 10> offsets{{
            {"LMT", 5596, false},
            {"IMT", 7016, false},
            {"EET", 7200, false},
            {"CET", 3600, false},
            {"CEST", 7200, true},
            {"EEST", 10800, true},
            {"EET", 7200, false},
            {"EEST", 10800, true},
            {"EEST", 10800, true},
            {"EET", 7200, false},
        }};

        inline constexpr array<zone::transition, 126> transitions{{
            {1, -2840146396},
            {2, -2369527016},
            {3, -857257200},
            {4, -844556400},
            {3, -828226800},
            {4, -812502000},
            {3, -796777200},
            {2, -781048800},
            {5, 291762000},
            {2, 307576800},
            {5, 323816400},
            {2, 339026400},
            {5, 355266000},
            {2, 370393200},
            {5, 386715600},
            {6, 401846400},
            {7, 417571200},
            {6, 433296000},
            {7, 449020800},
            {6, 465350400},
            {7, 481075200},
            {6, 496800000},
            {7, 512524800},
            {6, 528249600},
            {7, 543974400},
            {6, 559699200},
            {7, 575424000},
            {6, 591148800},
            {7, 606873600},
            {6, 622598400},
            {7, 638323200},
            {6, 654652800},
            {5, 670370400},
            {2, 686091600},
            {5, 701820000},
            {2, 717541200},
            {5, 733269600},
            {2, 748990800},
            {5, 764719200},
            {2, 780440400},
            {5, 796168800},
            {2, 811890000},
            {5, 828223200},
            {2, 846363600},
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

    inline constexpr db::entry sofia{"Europe/Sofia",
                                     "",
                                     "+4241+02319",
                                     detail::sofia::country_codes,
                                     detail::sofia::offsets,
                                     detail::sofia::transitions};

    // clang-format on
}
