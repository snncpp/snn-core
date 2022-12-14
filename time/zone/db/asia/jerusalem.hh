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

    namespace detail::jerusalem
    {
        inline constexpr array<country::code, 1> country_codes{
            "IL",
        };

        inline constexpr array<zone::offset, 9> offsets{{
            {"LMT", 8454, false},
            {"JMT", 8440, false},
            {"IDT", 10800, true},
            {"IST", 7200, false},
            {"IDDT", 14400, true},
            {"IDT", 10800, true},
            {"IST", 7200, false},
            {"IDT", 10800, true},
            {"IST", 7200, false},
        }};

        inline constexpr array<zone::transition, 149> transitions{{
            {1, -2840149254},
            {6, -1641003640},
            {2, -933638400},
            {3, -923097600},
            {2, -919036800},
            {3, -857347200},
            {2, -844300800},
            {3, -825811200},
            {2, -812678400},
            {3, -794188800},
            {2, -779846400},
            {3, -762652800},
            {2, -748310400},
            {3, -731116800},
            {4, -681955200},
            {2, -673228800},
            {3, -667958400},
            {2, -652320000},
            {3, -636422400},
            {2, -622080000},
            {3, -608947200},
            {2, -591840000},
            {3, -572486400},
            {2, -558576000},
            {3, -542851200},
            {2, -527731200},
            {3, -514425600},
            {2, -490838400},
            {3, -482976000},
            {2, -459388800},
            {3, -451526400},
            {2, -428544000},
            {3, -418262400},
            {2, -400118400},
            {3, -387417600},
            {5, 142380000},
            {6, 150843600},
            {5, 167176800},
            {6, 178664400},
            {7, 334101600},
            {8, 337730400},
            {7, 452642400},
            {8, 462319200},
            {5, 482277600},
            {6, 494370000},
            {5, 516751200},
            {6, 526424400},
            {5, 545436000},
            {6, 558478800},
            {5, 576626400},
            {6, 589323600},
            {5, 609890400},
            {6, 620773200},
            {5, 638316000},
            {6, 651618000},
            {5, 669765600},
            {6, 683672400},
            {5, 701820000},
            {6, 715726800},
            {5, 733701600},
            {6, 747176400},
            {5, 765151200},
            {6, 778021200},
            {5, 796600800},
            {6, 810075600},
            {5, 826840800},
            {6, 842821200},
            {5, 858895200},
            {6, 874184400},
            {5, 890344800},
            {6, 905029200},
            {5, 923011200},
            {6, 936313200},
            {5, 955670400},
            {6, 970783200},
            {5, 986770800},
            {6, 1001282400},
            {5, 1017356400},
            {6, 1033941600},
            {5, 1048806000},
            {6, 1065132000},
            {5, 1081292400},
            {6, 1095804000},
            {5, 1112313600},
            {6, 1128812400},
            {5, 1143763200},
            {6, 1159657200},
            {5, 1175212800},
            {6, 1189897200},
            {5, 1206662400},
            {6, 1223161200},
            {5, 1238112000},
            {6, 1254006000},
            {5, 1269561600},
            {6, 1284246000},
            {5, 1301616000},
            {6, 1317510000},
            {5, 1333065600},
            {6, 1348354800},
            {5, 1364515200},
            {6, 1382828400},
            {5, 1395964800},
            {6, 1414278000},
            {5, 1427414400},
            {6, 1445727600},
            {5, 1458864000},
            {6, 1477782000},
            {5, 1490313600},
            {6, 1509231600},
            {5, 1521763200},
            {6, 1540681200},
            {5, 1553817600},
            {6, 1572130800},
            {5, 1585267200},
            {6, 1603580400},
            {5, 1616716800},
            {6, 1635634800},
            {5, 1648166400},
            {6, 1667084400},
            {5, 1679616000},
            {6, 1698534000},
            {5, 1711670400},
            {6, 1729983600},
            {5, 1743120000},
            {6, 1761433200},
            {5, 1774569600},
            {6, 1792882800},
            {5, 1806019200},
            {6, 1824937200},
            {5, 1837468800},
            {6, 1856386800},
            {5, 1868918400},
            {6, 1887836400},
            {5, 1900972800},
            {6, 1919286000},
            {5, 1932422400},
            {6, 1950735600},
            {5, 1963872000},
            {6, 1982790000},
            {5, 1995321600},
            {6, 2014239600},
            {5, 2026771200},
            {6, 2045689200},
            {5, 2058220800},
            {6, 2077138800},
            {5, 2090275200},
            {6, 2108588400},
            {5, 2121724800},
            {6, 2140038000},
        }};
    }

    inline constexpr db::entry jerusalem{"Asia/Jerusalem",
                                         "",
                                         "+314650+0351326",
                                         detail::jerusalem::country_codes,
                                         detail::jerusalem::offsets,
                                         detail::jerusalem::transitions};

    // clang-format on
}
