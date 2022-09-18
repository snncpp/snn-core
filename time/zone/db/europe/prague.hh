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

    namespace detail::prague
    {
        inline constexpr array<country::code, 2> country_codes{
            "CZ",
            "SK",
        };

        inline constexpr array<zone::offset, 9> offsets{{
            {"LMT", 3464, false},
            {"PMT", 3464, false},
            {"CEST", 7200, true},
            {"CET", 3600, false},
            {"CEST", 7200, true},
            {"CET", 3600, false},
            {"GMT", 0, true},
            {"CEST", 7200, true},
            {"CET", 3600, false},
        }};

        inline constexpr array<zone::transition, 144> transitions{{
            {1, -3786829064},
            {3, -2469401864},
            {2, -1693706400},
            {3, -1680483600},
            {4, -1663455600},
            {5, -1650150000},
            {4, -1632006000},
            {5, -1618700400},
            {4, -938905200},
            {5, -857257200},
            {4, -844556400},
            {5, -828226800},
            {4, -812502000},
            {5, -796777200},
            {4, -781052400},
            {5, -765327600},
            {4, -746578800},
            {5, -733359600},
            {6, -728517600},
            {3, -721260000},
            {4, -716425200},
            {5, -701910000},
            {4, -684975600},
            {5, -670460400},
            {4, -654217200},
            {5, -639010800},
            {7, 291776400},
            {8, 307501200},
            {7, 323830800},
            {8, 338950800},
            {7, 354675600},
            {8, 370400400},
            {7, 386125200},
            {8, 401850000},
            {7, 417574800},
            {8, 433299600},
            {7, 449024400},
            {8, 465354000},
            {7, 481078800},
            {8, 496803600},
            {7, 512528400},
            {8, 528253200},
            {7, 543978000},
            {8, 559702800},
            {7, 575427600},
            {8, 591152400},
            {7, 606877200},
            {8, 622602000},
            {7, 638326800},
            {8, 654656400},
            {7, 670381200},
            {8, 686106000},
            {7, 701830800},
            {8, 717555600},
            {7, 733280400},
            {8, 749005200},
            {7, 764730000},
            {8, 780454800},
            {7, 796179600},
            {8, 811904400},
            {7, 828234000},
            {8, 846378000},
            {7, 859683600},
            {8, 877827600},
            {7, 891133200},
            {8, 909277200},
            {7, 922582800},
            {8, 941331600},
            {7, 954032400},
            {8, 972781200},
            {7, 985482000},
            {8, 1004230800},
            {7, 1017536400},
            {8, 1035680400},
            {7, 1048986000},
            {8, 1067130000},
            {7, 1080435600},
            {8, 1099184400},
            {7, 1111885200},
            {8, 1130634000},
            {7, 1143334800},
            {8, 1162083600},
            {7, 1174784400},
            {8, 1193533200},
            {7, 1206838800},
            {8, 1224982800},
            {7, 1238288400},
            {8, 1256432400},
            {7, 1269738000},
            {8, 1288486800},
            {7, 1301187600},
            {8, 1319936400},
            {7, 1332637200},
            {8, 1351386000},
            {7, 1364691600},
            {8, 1382835600},
            {7, 1396141200},
            {8, 1414285200},
            {7, 1427590800},
            {8, 1445734800},
            {7, 1459040400},
            {8, 1477789200},
            {7, 1490490000},
            {8, 1509238800},
            {7, 1521939600},
            {8, 1540688400},
            {7, 1553994000},
            {8, 1572138000},
            {7, 1585443600},
            {8, 1603587600},
            {7, 1616893200},
            {8, 1635642000},
            {7, 1648342800},
            {8, 1667091600},
            {7, 1679792400},
            {8, 1698541200},
            {7, 1711846800},
            {8, 1729990800},
            {7, 1743296400},
            {8, 1761440400},
            {7, 1774746000},
            {8, 1792890000},
            {7, 1806195600},
            {8, 1824944400},
            {7, 1837645200},
            {8, 1856394000},
            {7, 1869094800},
            {8, 1887843600},
            {7, 1901149200},
            {8, 1919293200},
            {7, 1932598800},
            {8, 1950742800},
            {7, 1964048400},
            {8, 1982797200},
            {7, 1995498000},
            {8, 2014246800},
            {7, 2026947600},
            {8, 2045696400},
            {7, 2058397200},
            {8, 2077146000},
            {7, 2090451600},
            {8, 2108595600},
            {7, 2121901200},
            {8, 2140045200},
        }};
    }

    inline constexpr db::entry prague{"Europe/Prague",
                                      "",
                                      "+5005+01426",
                                      detail::prague::country_codes,
                                      detail::prague::offsets,
                                      detail::prague::transitions};

    // clang-format on
}
