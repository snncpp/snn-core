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

    namespace detail::belgrade
    {
        inline constexpr array<country::code, 6> country_codes{
            "RS",
            "BA",
            "HR",
            "ME",
            "MK",
            "SI",
        };

        inline constexpr array<zone::offset, 7> offsets{{
            {"LMT", 4920, false},
            {"CET", 3600, false},
            {"CET", 3600, false},
            {"CEST", 7200, true},
            {"CEST", 7200, true},
            {"CEST", 7200, true},
            {"CET", 3600, false},
        }};

        inline constexpr array<zone::transition, 119> transitions{{
            {1, -2713915320},
            {4, -905824800},
            {2, -857257200},
            {3, -844556400},
            {2, -828226800},
            {3, -812502000},
            {2, -796777200},
            {3, -777942000},
            {2, -766623600},
            {5, 417574800},
            {6, 433299600},
            {5, 449024400},
            {6, 465354000},
            {5, 481078800},
            {6, 496803600},
            {5, 512528400},
            {6, 528253200},
            {5, 543978000},
            {6, 559702800},
            {5, 575427600},
            {6, 591152400},
            {5, 606877200},
            {6, 622602000},
            {5, 638326800},
            {6, 654656400},
            {5, 670381200},
            {6, 686106000},
            {5, 701830800},
            {6, 717555600},
            {5, 733280400},
            {6, 749005200},
            {5, 764730000},
            {6, 780454800},
            {5, 796179600},
            {6, 811904400},
            {5, 828234000},
            {6, 846378000},
            {5, 859683600},
            {6, 877827600},
            {5, 891133200},
            {6, 909277200},
            {5, 922582800},
            {6, 941331600},
            {5, 954032400},
            {6, 972781200},
            {5, 985482000},
            {6, 1004230800},
            {5, 1017536400},
            {6, 1035680400},
            {5, 1048986000},
            {6, 1067130000},
            {5, 1080435600},
            {6, 1099184400},
            {5, 1111885200},
            {6, 1130634000},
            {5, 1143334800},
            {6, 1162083600},
            {5, 1174784400},
            {6, 1193533200},
            {5, 1206838800},
            {6, 1224982800},
            {5, 1238288400},
            {6, 1256432400},
            {5, 1269738000},
            {6, 1288486800},
            {5, 1301187600},
            {6, 1319936400},
            {5, 1332637200},
            {6, 1351386000},
            {5, 1364691600},
            {6, 1382835600},
            {5, 1396141200},
            {6, 1414285200},
            {5, 1427590800},
            {6, 1445734800},
            {5, 1459040400},
            {6, 1477789200},
            {5, 1490490000},
            {6, 1509238800},
            {5, 1521939600},
            {6, 1540688400},
            {5, 1553994000},
            {6, 1572138000},
            {5, 1585443600},
            {6, 1603587600},
            {5, 1616893200},
            {6, 1635642000},
            {5, 1648342800},
            {6, 1667091600},
            {5, 1679792400},
            {6, 1698541200},
            {5, 1711846800},
            {6, 1729990800},
            {5, 1743296400},
            {6, 1761440400},
            {5, 1774746000},
            {6, 1792890000},
            {5, 1806195600},
            {6, 1824944400},
            {5, 1837645200},
            {6, 1856394000},
            {5, 1869094800},
            {6, 1887843600},
            {5, 1901149200},
            {6, 1919293200},
            {5, 1932598800},
            {6, 1950742800},
            {5, 1964048400},
            {6, 1982797200},
            {5, 1995498000},
            {6, 2014246800},
            {5, 2026947600},
            {6, 2045696400},
            {5, 2058397200},
            {6, 2077146000},
            {5, 2090451600},
            {6, 2108595600},
            {5, 2121901200},
            {6, 2140045200},
        }};
    }

    inline constexpr db::entry belgrade{"Europe/Belgrade",
                                        "",
                                        "+4450+02030",
                                        detail::belgrade::country_codes,
                                        detail::belgrade::offsets,
                                        detail::belgrade::transitions};

    // clang-format on
}