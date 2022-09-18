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

    namespace detail::gaza
    {
        inline constexpr array<country::code, 1> country_codes{
            "PS",
        };

        inline constexpr array<zone::offset, 10> offsets{{
            {"LMT", 8272, false},
            {"EEST", 10800, true},
            {"EET", 7200, false},
            {"EET", 7200, false},
            {"EEST", 10800, true},
            {"IDT", 10800, true},
            {"IST", 7200, false},
            {"IDT", 10800, true},
            {"IST", 7200, false},
            {"EET", 7200, false},
        }};

        inline constexpr array<zone::transition, 150> transitions{{
            {3, -2185409872},
            {1, -933638400},
            {2, -923097600},
            {1, -919036800},
            {2, -857347200},
            {1, -844300800},
            {2, -825811200},
            {1, -812678400},
            {2, -794188800},
            {1, -779846400},
            {2, -762652800},
            {1, -748310400},
            {2, -731116800},
            {4, -399088800},
            {3, -386650800},
            {4, -368330400},
            {3, -355114800},
            {4, -336790800},
            {3, -323654400},
            {4, -305168400},
            {3, -292032000},
            {4, -273632400},
            {3, -260496000},
            {4, -242096400},
            {3, -228960000},
            {4, -210560400},
            {3, -197424000},
            {4, -178938000},
            {3, -165801600},
            {4, -147402000},
            {3, -134265600},
            {4, -115866000},
            {3, -102643200},
            {4, -84330000},
            {6, -81313200},
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
            {3, 820447200},
            {4, 828655200},
            {9, 843170400},
            {4, 860104800},
            {9, 874620000},
            {4, 891554400},
            {9, 906069600},
            {4, 924213600},
            {3, 939934800},
            {4, 956268000},
            {3, 971989200},
            {4, 987717600},
            {3, 1003438800},
            {4, 1019167200},
            {3, 1034888400},
            {4, 1050616800},
            {3, 1066338000},
            {4, 1082066400},
            {3, 1096581600},
            {4, 1113516000},
            {3, 1128380400},
            {4, 1143842400},
            {3, 1158872400},
            {4, 1175378400},
            {3, 1189638000},
            {4, 1206655200},
            {3, 1219957200},
            {4, 1238104800},
            {3, 1252015200},
            {4, 1269640860},
            {3, 1281474000},
            {4, 1301608860},
            {3, 1312146000},
            {4, 1333058400},
            {3, 1348178400},
            {4, 1364508000},
            {3, 1380229200},
            {4, 1395957600},
            {3, 1414098000},
            {4, 1427493600},
            {3, 1445551200},
            {4, 1458946800},
            {3, 1477692000},
            {4, 1490396400},
            {3, 1509141600},
            {4, 1521846000},
            {3, 1540591200},
            {4, 1553810400},
            {3, 1572037200},
            {4, 1585346400},
            {3, 1603490400},
            {4, 1616796000},
            {3, 1635458400},
            {4, 1648332000},
            {3, 1666908000},
            {4, 1679781600},
            {3, 1698357600},
            {4, 1711836000},
            {3, 1729807200},
            {4, 1743285600},
            {3, 1761256800},
            {4, 1774735200},
            {3, 1792706400},
            {4, 1806184800},
            {3, 1824760800},
            {4, 1837634400},
            {3, 1856210400},
            {4, 1869084000},
            {3, 1887660000},
            {4, 1901138400},
            {3, 1919109600},
            {4, 1932588000},
            {3, 1950559200},
            {4, 1964037600},
            {3, 1982613600},
            {4, 1995487200},
            {3, 2014063200},
            {4, 2026936800},
            {3, 2045512800},
            {4, 2058386400},
            {3, 2076962400},
            {4, 2090440800},
            {3, 2108412000},
            {4, 2121890400},
            {3, 2139861600},
        }};
    }

    inline constexpr db::entry gaza{"Asia/Gaza",
                                    "Gaza Strip",
                                    "+3130+03428",
                                    detail::gaza::country_codes,
                                    detail::gaza::offsets,
                                    detail::gaza::transitions};

    // clang-format on
}
