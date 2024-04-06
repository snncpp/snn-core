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

        inline constexpr array<zone::transition, 308> transitions{{
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
            {3, 1666998000},
            {4, 1682726400},
            {3, 1698447600},
            {4, 1713571200},
            {3, 1729897200},
            {4, 1744416000},
            {3, 1761346800},
            {4, 1774656000},
            {3, 1792796400},
            {4, 1806105600},
            {3, 1824850800},
            {4, 1837555200},
            {3, 1856300400},
            {4, 1869004800},
            {3, 1887750000},
            {4, 1901059200},
            {3, 1919199600},
            {4, 1932508800},
            {3, 1950649200},
            {4, 1963958400},
            {3, 1982703600},
            {4, 1995408000},
            {3, 2014153200},
            {4, 2026857600},
            {3, 2045602800},
            {4, 2058307200},
            {3, 2077052400},
            {4, 2090361600},
            {3, 2107897200},
            {4, 2121811200},
            {3, 2138742000},
            {4, 2153260800},
            {3, 2168982000},
            {4, 2184710400},
            {3, 2199826800},
            {4, 2216160000},
            {3, 2230066800},
            {4, 2234304000},
            {3, 2234905200},
            {4, 2248214400},
            {3, 2260911600},
            {4, 2264544000},
            {3, 2266354800},
            {4, 2279664000},
            {3, 2291756400},
            {4, 2295388800},
            {3, 2297804400},
            {4, 2311113600},
            {3, 2321996400},
            {4, 2326233600},
            {3, 2329254000},
            {4, 2342563200},
            {3, 2352841200},
            {4, 2356473600},
            {3, 2361308400},
            {4, 2374012800},
            {3, 2383686000},
            {4, 2387318400},
            {3, 2392758000},
            {4, 2405462400},
            {3, 2413926000},
            {4, 2418163200},
            {3, 2424207600},
            {4, 2437516800},
            {3, 2444770800},
            {4, 2448403200},
            {3, 2455657200},
            {4, 2468966400},
            {3, 2475010800},
            {4, 2479248000},
            {3, 2487106800},
            {4, 2500416000},
            {3, 2505855600},
            {4, 2509488000},
            {3, 2519161200},
            {4, 2531865600},
            {3, 2536700400},
            {4, 2540332800},
            {3, 2550610800},
            {4, 2563315200},
            {3, 2566940400},
            {4, 2571177600},
            {3, 2582060400},
            {4, 2595369600},
            {3, 2597785200},
            {4, 2601417600},
            {3, 2613510000},
            {4, 2626819200},
            {3, 2628025200},
            {4, 2632262400},
            {3, 2644959600},
            {4, 2658268800},
            {3, 2658870000},
            {4, 2663107200},
            {3, 2676409200},
            {4, 2693347200},
            {3, 2708463600},
            {4, 2724192000},
            {3, 2739913200},
            {4, 2754432000},
            {3, 2771362800},
            {4, 2785276800},
            {3, 2802812400},
            {4, 2816121600},
            {3, 2834262000},
            {4, 2847571200},
            {3, 2866316400},
            {4, 2879020800},
            {3, 2897766000},
            {4, 2910470400},
            {3, 2929215600},
            {4, 2941920000},
            {3, 2960665200},
            {4, 2973974400},
            {3, 2992114800},
            {4, 3005424000},
            {3, 3023564400},
            {4, 3036873600},
            {3, 3055618800},
            {4, 3068323200},
            {3, 3087068400},
            {4, 3099772800},
            {3, 3117913200},
            {4, 3131827200},
            {3, 3148758000},
            {4, 3163276800},
            {3, 3179602800},
            {4, 3194726400},
            {3, 3209842800},
            {4, 3226176000},
            {3, 3240687600},
            {4, 3244320000},
            {3, 3244921200},
            {4, 3257625600},
            {3, 3271532400},
            {4, 3275164800},
            {3, 3276370800},
            {4, 3289075200},
            {3, 3301772400},
            {4, 3306009600},
            {3, 3307820400},
            {4, 3321129600},
            {3, 3332617200},
            {4, 3336249600},
            {3, 3339270000},
            {4, 3352579200},
            {3, 3362857200},
            {4, 3367094400},
            {3, 3370719600},
            {4, 3384028800},
            {3, 3393702000},
            {4, 3397939200},
            {3, 3402774000},
            {4, 3415478400},
            {3, 3424546800},
            {4, 3428179200},
            {3, 3434223600},
            {4, 3446928000},
            {3, 3454786800},
            {4, 3459024000},
            {3, 3465673200},
            {4, 3478982400},
            {3, 3485631600},
            {4, 3489264000},
            {3, 3497122800},
            {4, 3510432000},
            {3, 3516476400},
            {4, 3520108800},
            {3, 3528572400},
            {4, 3541881600},
            {3, 3546716400},
            {4, 3550953600},
            {3, 3560022000},
            {4, 3573331200},
            {3, 3577561200},
            {4, 3581193600},
            {3, 3592076400},
            {4, 3604780800},
            {3, 3607801200},
            {4, 3612038400},
            {3, 3623526000},
            {4, 3636230400},
            {3, 3638646000},
            {4, 3642883200},
            {3, 3654975600},
            {4, 3668284800},
            {3, 3669490800},
            {4, 3673123200},
            {3, 3686425200},
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
