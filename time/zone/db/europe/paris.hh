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

    namespace detail::paris
    {
        inline constexpr array<country::code, 2> country_codes{
            "FR",
            "MC",
        };

        inline constexpr array<zone::offset, 13> offsets{{
            {"LMT", 561, false},
            {"PMT", 561, false},
            {"WEST", 3600, true},
            {"WET", 0, false},
            {"WEST", 3600, true},
            {"WET", 0, false},
            {"CET", 3600, false},
            {"CEST", 7200, true},
            {"CEST", 7200, true},
            {"WEMT", 7200, true},
            {"CET", 3600, false},
            {"CEST", 7200, true},
            {"CET", 3600, false},
        }};

        inline constexpr array<zone::transition, 184> transitions{{
            {1, -2486592561},
            {5, -1855958961},
            {2, -1689814800},
            {3, -1680397200},
            {2, -1665363600},
            {3, -1648342800},
            {2, -1635123600},
            {3, -1616893200},
            {2, -1604278800},
            {3, -1585443600},
            {2, -1574038800},
            {3, -1552266000},
            {2, -1539997200},
            {3, -1520557200},
            {2, -1507510800},
            {3, -1490576400},
            {2, -1470618000},
            {3, -1459126800},
            {2, -1444006800},
            {3, -1427677200},
            {2, -1411952400},
            {3, -1396227600},
            {2, -1379293200},
            {3, -1364778000},
            {2, -1348448400},
            {3, -1333328400},
            {2, -1316394000},
            {3, -1301274000},
            {2, -1284339600},
            {3, -1269824400},
            {2, -1253494800},
            {3, -1238374800},
            {2, -1221440400},
            {3, -1206925200},
            {2, -1191200400},
            {3, -1175475600},
            {2, -1160355600},
            {3, -1143421200},
            {2, -1127696400},
            {3, -1111971600},
            {2, -1096851600},
            {3, -1080522000},
            {2, -1063587600},
            {3, -1049072400},
            {2, -1033347600},
            {3, -1017622800},
            {2, -1002502800},
            {3, -986173200},
            {2, -969238800},
            {3, -950490000},
            {4, -942012000},
            {8, -932436000},
            {6, -857257200},
            {7, -844556400},
            {6, -828226800},
            {7, -812502000},
            {9, -800071200},
            {4, -796266000},
            {9, -781052400},
            {10, -766623600},
            {8, 196819200},
            {10, 212540400},
            {11, 228877200},
            {12, 243997200},
            {11, 260326800},
            {12, 276051600},
            {11, 291776400},
            {12, 307501200},
            {11, 323830800},
            {12, 338950800},
            {11, 354675600},
            {12, 370400400},
            {11, 386125200},
            {12, 401850000},
            {11, 417574800},
            {12, 433299600},
            {11, 449024400},
            {12, 465354000},
            {11, 481078800},
            {12, 496803600},
            {11, 512528400},
            {12, 528253200},
            {11, 543978000},
            {12, 559702800},
            {11, 575427600},
            {12, 591152400},
            {11, 606877200},
            {12, 622602000},
            {11, 638326800},
            {12, 654656400},
            {11, 670381200},
            {12, 686106000},
            {11, 701830800},
            {12, 717555600},
            {11, 733280400},
            {12, 749005200},
            {11, 764730000},
            {12, 780454800},
            {11, 796179600},
            {12, 811904400},
            {11, 828234000},
            {12, 846378000},
            {11, 859683600},
            {12, 877827600},
            {11, 891133200},
            {12, 909277200},
            {11, 922582800},
            {12, 941331600},
            {11, 954032400},
            {12, 972781200},
            {11, 985482000},
            {12, 1004230800},
            {11, 1017536400},
            {12, 1035680400},
            {11, 1048986000},
            {12, 1067130000},
            {11, 1080435600},
            {12, 1099184400},
            {11, 1111885200},
            {12, 1130634000},
            {11, 1143334800},
            {12, 1162083600},
            {11, 1174784400},
            {12, 1193533200},
            {11, 1206838800},
            {12, 1224982800},
            {11, 1238288400},
            {12, 1256432400},
            {11, 1269738000},
            {12, 1288486800},
            {11, 1301187600},
            {12, 1319936400},
            {11, 1332637200},
            {12, 1351386000},
            {11, 1364691600},
            {12, 1382835600},
            {11, 1396141200},
            {12, 1414285200},
            {11, 1427590800},
            {12, 1445734800},
            {11, 1459040400},
            {12, 1477789200},
            {11, 1490490000},
            {12, 1509238800},
            {11, 1521939600},
            {12, 1540688400},
            {11, 1553994000},
            {12, 1572138000},
            {11, 1585443600},
            {12, 1603587600},
            {11, 1616893200},
            {12, 1635642000},
            {11, 1648342800},
            {12, 1667091600},
            {11, 1679792400},
            {12, 1698541200},
            {11, 1711846800},
            {12, 1729990800},
            {11, 1743296400},
            {12, 1761440400},
            {11, 1774746000},
            {12, 1792890000},
            {11, 1806195600},
            {12, 1824944400},
            {11, 1837645200},
            {12, 1856394000},
            {11, 1869094800},
            {12, 1887843600},
            {11, 1901149200},
            {12, 1919293200},
            {11, 1932598800},
            {12, 1950742800},
            {11, 1964048400},
            {12, 1982797200},
            {11, 1995498000},
            {12, 2014246800},
            {11, 2026947600},
            {12, 2045696400},
            {11, 2058397200},
            {12, 2077146000},
            {11, 2090451600},
            {12, 2108595600},
            {11, 2121901200},
            {12, 2140045200},
        }};
    }

    inline constexpr db::entry paris{"Europe/Paris",
                                     "",
                                     "+4852+00220",
                                     detail::paris::country_codes,
                                     detail::paris::offsets,
                                     detail::paris::transitions};

    // clang-format on
}
