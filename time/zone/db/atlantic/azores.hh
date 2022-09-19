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

namespace snn::time::zone::db::atlantic
{
    // clang-format off

    namespace detail::azores
    {
        inline constexpr array<country::code, 1> country_codes{
            "PT",
        };

        inline constexpr array<zone::offset, 13> offsets{{
            {"LMT", -6160, false},
            {"HMT", -6872, false},
            {"-01", -3600, true},
            {"-02", -7200, false},
            {"-01", -3600, true},
            {"-02", -7200, false},
            {"-02", -7200, false},
            {"+00", 0, true},
            {"-01", -3600, false},
            {"-01", -3600, false},
            {"WET", 0, false},
            {"+00", 0, true},
            {"-01", -3600, false},
        }};

        inline constexpr array<zone::transition, 224> transitions{{
            {1, -2713904240},
            {6, -1830376800},
            {2, -1689548400},
            {3, -1677794400},
            {4, -1667430000},
            {5, -1647730800},
            {4, -1635807600},
            {5, -1616194800},
            {4, -1604358000},
            {5, -1584658800},
            {4, -1572735600},
            {5, -1553036400},
            {4, -1541199600},
            {5, -1521500400},
            {4, -1442444400},
            {5, -1426806000},
            {4, -1379286000},
            {5, -1364770800},
            {4, -1348441200},
            {5, -1333321200},
            {4, -1316386800},
            {5, -1301266800},
            {4, -1284332400},
            {5, -1269817200},
            {4, -1221433200},
            {5, -1206918000},
            {4, -1191193200},
            {5, -1175468400},
            {4, -1127689200},
            {5, -1111964400},
            {4, -1096844400},
            {5, -1080514800},
            {4, -1063580400},
            {5, -1049065200},
            {4, -1033340400},
            {5, -1017615600},
            {4, -1002495600},
            {5, -986166000},
            {4, -969231600},
            {5, -950482800},
            {4, -942015600},
            {5, -922662000},
            {4, -906937200},
            {5, -891126000},
            {4, -877302000},
            {7, -873676800},
            {4, -864000000},
            {5, -857948400},
            {4, -845852400},
            {7, -842832000},
            {4, -831340800},
            {5, -825894000},
            {4, -814402800},
            {7, -810777600},
            {4, -799891200},
            {5, -794444400},
            {4, -782953200},
            {7, -779328000},
            {4, -768441600},
            {5, -762994800},
            {4, -749084400},
            {5, -733359600},
            {4, -717624000},
            {5, -701899200},
            {4, -686174400},
            {5, -670449600},
            {4, -654724800},
            {5, -639000000},
            {4, -623275200},
            {5, -607550400},
            {4, -591825600},
            {5, -575496000},
            {4, -559771200},
            {5, -544046400},
            {4, -528321600},
            {5, -512596800},
            {4, -496872000},
            {5, -481147200},
            {4, -465422400},
            {5, -449697600},
            {4, -433972800},
            {5, -417643200},
            {4, -401918400},
            {5, -386193600},
            {4, -370468800},
            {5, -354744000},
            {4, -339019200},
            {5, -323294400},
            {4, -307569600},
            {5, -291844800},
            {4, -276120000},
            {5, -260395200},
            {4, -244670400},
            {5, -228340800},
            {4, -212616000},
            {5, -196891200},
            {4, -181166400},
            {5, -165441600},
            {4, -149716800},
            {5, -133992000},
            {9, -118267200},
            {7, 228272400},
            {8, 243997200},
            {7, 260326800},
            {8, 276051600},
            {7, 291776400},
            {8, 307504800},
            {7, 323226000},
            {8, 338954400},
            {7, 354679200},
            {8, 370404000},
            {7, 386128800},
            {8, 401853600},
            {7, 417582000},
            {8, 433303200},
            {7, 449028000},
            {8, 465357600},
            {7, 481082400},
            {8, 496807200},
            {7, 512532000},
            {8, 528256800},
            {7, 543981600},
            {8, 559706400},
            {7, 575431200},
            {8, 591156000},
            {7, 606880800},
            {8, 622605600},
            {7, 638330400},
            {8, 654660000},
            {7, 670384800},
            {8, 686109600},
            {7, 701834400},
            {10, 717559200},
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
            {12, 2147483647},
        }};
    }

    inline constexpr db::entry azores{"Atlantic/Azores",
                                      "Azores",
                                      "+3744-02540",
                                      detail::azores::country_codes,
                                      detail::azores::offsets,
                                      detail::azores::transitions};

    // clang-format on
}