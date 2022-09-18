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

namespace snn::time::zone::db::antarctica
{
    // clang-format off

    namespace detail::troll
    {
        inline constexpr array<country::code, 1> country_codes{
            "AQ",
        };

        inline constexpr array<zone::offset, 4> offsets{{
            {"-00", 0, false},
            {"+02", 7200, true},
            {"+00", 0, false},
            {"+00", 0, false},
        }};

        inline constexpr array<zone::transition, 68> transitions{{
            {3, 1108166400},
            {1, 1111885200},
            {2, 1130634000},
            {1, 1143334800},
            {2, 1162083600},
            {1, 1174784400},
            {2, 1193533200},
            {1, 1206838800},
            {2, 1224982800},
            {1, 1238288400},
            {2, 1256432400},
            {1, 1269738000},
            {2, 1288486800},
            {1, 1301187600},
            {2, 1319936400},
            {1, 1332637200},
            {2, 1351386000},
            {1, 1364691600},
            {2, 1382835600},
            {1, 1396141200},
            {2, 1414285200},
            {1, 1427590800},
            {2, 1445734800},
            {1, 1459040400},
            {2, 1477789200},
            {1, 1490490000},
            {2, 1509238800},
            {1, 1521939600},
            {2, 1540688400},
            {1, 1553994000},
            {2, 1572138000},
            {1, 1585443600},
            {2, 1603587600},
            {1, 1616893200},
            {2, 1635642000},
            {1, 1648342800},
            {2, 1667091600},
            {1, 1679792400},
            {2, 1698541200},
            {1, 1711846800},
            {2, 1729990800},
            {1, 1743296400},
            {2, 1761440400},
            {1, 1774746000},
            {2, 1792890000},
            {1, 1806195600},
            {2, 1824944400},
            {1, 1837645200},
            {2, 1856394000},
            {1, 1869094800},
            {2, 1887843600},
            {1, 1901149200},
            {2, 1919293200},
            {1, 1932598800},
            {2, 1950742800},
            {1, 1964048400},
            {2, 1982797200},
            {1, 1995498000},
            {2, 2014246800},
            {1, 2026947600},
            {2, 2045696400},
            {1, 2058397200},
            {2, 2077146000},
            {1, 2090451600},
            {2, 2108595600},
            {1, 2121901200},
            {2, 2140045200},
            {2, 2147483647},
        }};
    }

    inline constexpr db::entry troll{"Antarctica/Troll",
                                     "Troll",
                                     "-720041+0023206",
                                     detail::troll::country_codes,
                                     detail::troll::offsets,
                                     detail::troll::transitions};

    // clang-format on
}
