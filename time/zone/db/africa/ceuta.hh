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

namespace snn::time::zone::db::africa
{
    // clang-format off

    namespace detail::ceuta
    {
        inline constexpr array<country::code, 1> country_codes{
            "ES",
        };

        inline constexpr array<zone::offset, 8> offsets{{
            {"LMT", -1276, false},
            {"WET", 0, false},
            {"WEST", 3600, true},
            {"WET", 0, false},
            {"WET", 0, false},
            {"CET", 3600, false},
            {"CEST", 7200, true},
            {"CET", 3600, false},
        }};

        inline constexpr array<zone::transition, 126> transitions{{
            {1, -2177452800},
            {2, -1630112400},
            {3, -1616810400},
            {2, -1442451600},
            {4, -1427673600},
            {2, -1379293200},
            {4, -1364774400},
            {2, -1348448400},
            {4, -1333324800},
            {2, -1316390400},
            {4, -1301270400},
            {2, -81432000},
            {3, -71110800},
            {2, 141264000},
            {3, 147222000},
            {2, 199756800},
            {3, 207702000},
            {2, 231292800},
            {3, 244249200},
            {2, 265507200},
            {3, 271033200},
            {5, 448243200},
            {6, 512528400},
            {7, 528253200},
            {6, 543978000},
            {7, 559702800},
            {6, 575427600},
            {7, 591152400},
            {6, 606877200},
            {7, 622602000},
            {6, 638326800},
            {7, 654656400},
            {6, 670381200},
            {7, 686106000},
            {6, 701830800},
            {7, 717555600},
            {6, 733280400},
            {7, 749005200},
            {6, 764730000},
            {7, 780454800},
            {6, 796179600},
            {7, 811904400},
            {6, 828234000},
            {7, 846378000},
            {6, 859683600},
            {7, 877827600},
            {6, 891133200},
            {7, 909277200},
            {6, 922582800},
            {7, 941331600},
            {6, 954032400},
            {7, 972781200},
            {6, 985482000},
            {7, 1004230800},
            {6, 1017536400},
            {7, 1035680400},
            {6, 1048986000},
            {7, 1067130000},
            {6, 1080435600},
            {7, 1099184400},
            {6, 1111885200},
            {7, 1130634000},
            {6, 1143334800},
            {7, 1162083600},
            {6, 1174784400},
            {7, 1193533200},
            {6, 1206838800},
            {7, 1224982800},
            {6, 1238288400},
            {7, 1256432400},
            {6, 1269738000},
            {7, 1288486800},
            {6, 1301187600},
            {7, 1319936400},
            {6, 1332637200},
            {7, 1351386000},
            {6, 1364691600},
            {7, 1382835600},
            {6, 1396141200},
            {7, 1414285200},
            {6, 1427590800},
            {7, 1445734800},
            {6, 1459040400},
            {7, 1477789200},
            {6, 1490490000},
            {7, 1509238800},
            {6, 1521939600},
            {7, 1540688400},
            {6, 1553994000},
            {7, 1572138000},
            {6, 1585443600},
            {7, 1603587600},
            {6, 1616893200},
            {7, 1635642000},
            {6, 1648342800},
            {7, 1667091600},
            {6, 1679792400},
            {7, 1698541200},
            {6, 1711846800},
            {7, 1729990800},
            {6, 1743296400},
            {7, 1761440400},
            {6, 1774746000},
            {7, 1792890000},
            {6, 1806195600},
            {7, 1824944400},
            {6, 1837645200},
            {7, 1856394000},
            {6, 1869094800},
            {7, 1887843600},
            {6, 1901149200},
            {7, 1919293200},
            {6, 1932598800},
            {7, 1950742800},
            {6, 1964048400},
            {7, 1982797200},
            {6, 1995498000},
            {7, 2014246800},
            {6, 2026947600},
            {7, 2045696400},
            {6, 2058397200},
            {7, 2077146000},
            {6, 2090451600},
            {7, 2108595600},
            {6, 2121901200},
            {7, 2140045200},
        }};
    }

    inline constexpr db::entry ceuta{"Africa/Ceuta",
                                     "Ceuta, Melilla",
                                     "+3553-00519",
                                     detail::ceuta::country_codes,
                                     detail::ceuta::offsets,
                                     detail::ceuta::transitions};

    // clang-format on
}
