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

namespace snn::time::zone::db::asia
{
    // clang-format off

    namespace detail::amman
    {
        inline constexpr array<country::code, 1> country_codes{
            "JO",
        };

        inline constexpr array<zone::offset, 5> offsets{{
            {"LMT", 8624, false},
            {"EEST", 10800, true},
            {"EET", 7200, false},
            {"EET", 7200, false},
            {"EEST", 10800, true},
        }};

        inline constexpr array<zone::transition, 117> transitions{{
            {2, -1230776624},
            {1, 108165600},
            {2, 118270800},
            {1, 136591200},
            {2, 149806800},
            {1, 168127200},
            {2, 181342800},
            {1, 199749600},
            {2, 215643600},
            {1, 231285600},
            {2, 244501200},
            {1, 262735200},
            {2, 275950800},
            {1, 481154400},
            {2, 496962000},
            {1, 512949600},
            {2, 528670800},
            {1, 544399200},
            {2, 560120400},
            {1, 575848800},
            {2, 592174800},
            {1, 610581600},
            {2, 623624400},
            {1, 641167200},
            {2, 655074000},
            {1, 671839200},
            {2, 685918800},
            {1, 702856800},
            {2, 717973200},
            {1, 733701600},
            {2, 749422800},
            {1, 765151200},
            {2, 779662800},
            {1, 797205600},
            {3, 811116000},
            {1, 828655200},
            {3, 843170400},
            {1, 860104800},
            {3, 874620000},
            {1, 891554400},
            {3, 906069600},
            {4, 930780000},
            {3, 938124000},
            {4, 954367200},
            {3, 970178400},
            {4, 985816800},
            {3, 1001628000},
            {1, 1017352800},
            {3, 1033077600},
            {1, 1048802400},
            {3, 1066946400},
            {1, 1080252000},
            {3, 1097791200},
            {1, 1112306400},
            {3, 1128031200},
            {1, 1143756000},
            {3, 1161900000},
            {1, 1175205600},
            {3, 1193349600},
            {1, 1206655200},
            {3, 1225404000},
            {1, 1238104800},
            {3, 1256853600},
            {1, 1269554400},
            {3, 1288303200},
            {1, 1301608800},
            {3, 1319752800},
            {1, 1333058400},
            {2, 1387486800},
            {1, 1395957600},
            {3, 1414706400},
            {1, 1427407200},
            {3, 1446156000},
            {1, 1459461600},
            {3, 1477605600},
            {1, 1490911200},
            {3, 1509055200},
            {1, 1522360800},
            {3, 1540504800},
            {1, 1553810400},
            {3, 1571954400},
            {1, 1585260000},
            {3, 1604008800},
            {1, 1616709600},
            {3, 1635458400},
            {1, 1645740000},
            {3, 1666908000},
            {1, 1677189600},
            {3, 1698357600},
            {1, 1709244000},
            {3, 1729807200},
            {1, 1740693600},
            {3, 1761861600},
            {1, 1772143200},
            {3, 1793311200},
            {1, 1803592800},
            {3, 1824760800},
            {1, 1835042400},
            {3, 1856210400},
            {1, 1866492000},
            {3, 1887660000},
            {1, 1898546400},
            {3, 1919109600},
            {1, 1929996000},
            {3, 1951164000},
            {1, 1961445600},
            {3, 1982613600},
            {1, 1992895200},
            {3, 2014063200},
            {1, 2024344800},
            {3, 2045512800},
            {1, 2055794400},
            {3, 2076962400},
            {1, 2087848800},
            {3, 2109016800},
            {1, 2119298400},
            {3, 2140466400},
        }};
    }

    inline constexpr db::entry amman{"Asia/Amman",
                                     "",
                                     "+3157+03556",
                                     detail::amman::country_codes,
                                     detail::amman::offsets,
                                     detail::amman::transitions};

    // clang-format on
}
