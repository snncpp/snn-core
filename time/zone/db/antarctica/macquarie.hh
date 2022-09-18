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

    namespace detail::macquarie
    {
        inline constexpr array<country::code, 1> country_codes{
            "AU",
        };

        inline constexpr array<zone::offset, 7> offsets{{
            {"-00", 0, false},
            {"AEST", 36000, false},
            {"AEDT", 39600, true},
            {"AEST", 36000, false},
            {"-00", 0, false},
            {"AEDT", 39600, true},
            {"AEST", 36000, false},
        }};

        inline constexpr array<zone::transition, 144> transitions{{
            {1, -2214259200},
            {2, -1680508800},
            {3, -1665388800},
            {4, -1601719200},
            {1, -687052800},
            {5, -71136000},
            {3, -55411200},
            {5, -37267200},
            {3, -25776000},
            {5, -5817600},
            {3, 5673600},
            {5, 25632000},
            {3, 37728000},
            {5, 57686400},
            {3, 67968000},
            {5, 89136000},
            {3, 100022400},
            {5, 120585600},
            {3, 131472000},
            {5, 152035200},
            {3, 162921600},
            {5, 183484800},
            {3, 194976000},
            {5, 215539200},
            {3, 226425600},
            {5, 246988800},
            {3, 257875200},
            {5, 278438400},
            {3, 289324800},
            {5, 309888000},
            {3, 320774400},
            {5, 341337600},
            {3, 352224000},
            {5, 372787200},
            {3, 386092800},
            {5, 404841600},
            {3, 417542400},
            {5, 436291200},
            {3, 447177600},
            {5, 467740800},
            {3, 478627200},
            {5, 499190400},
            {3, 510076800},
            {5, 530035200},
            {3, 542736000},
            {5, 562089600},
            {3, 574790400},
            {5, 594144000},
            {3, 606240000},
            {5, 625593600},
            {3, 637689600},
            {5, 657043200},
            {3, 670348800},
            {5, 686678400},
            {3, 701798400},
            {5, 718128000},
            {3, 733248000},
            {5, 749577600},
            {3, 764697600},
            {5, 781027200},
            {3, 796147200},
            {5, 812476800},
            {3, 828201600},
            {5, 844531200},
            {3, 859651200},
            {5, 875980800},
            {3, 891100800},
            {5, 907430400},
            {3, 922550400},
            {5, 938880000},
            {3, 954000000},
            {5, 967305600},
            {3, 985449600},
            {5, 1002384000},
            {3, 1017504000},
            {5, 1033833600},
            {3, 1048953600},
            {5, 1065283200},
            {3, 1080403200},
            {5, 1096732800},
            {3, 1111852800},
            {5, 1128182400},
            {3, 1143907200},
            {5, 1159632000},
            {3, 1174752000},
            {5, 1191686400},
            {3, 1207411200},
            {5, 1223136000},
            {3, 1238860800},
            {5, 1254585600},
            {3, 1301760000},
            {5, 1317484800},
            {3, 1333209600},
            {5, 1349539200},
            {3, 1365264000},
            {5, 1380988800},
            {3, 1396713600},
            {5, 1412438400},
            {3, 1428163200},
            {5, 1443888000},
            {3, 1459612800},
            {5, 1475337600},
            {3, 1491062400},
            {5, 1506787200},
            {3, 1522512000},
            {5, 1538841600},
            {3, 1554566400},
            {5, 1570291200},
            {3, 1586016000},
            {5, 1601740800},
            {3, 1617465600},
            {5, 1633190400},
            {3, 1648915200},
            {5, 1664640000},
            {3, 1680364800},
            {5, 1696089600},
            {3, 1712419200},
            {5, 1728144000},
            {3, 1743868800},
            {5, 1759593600},
            {3, 1775318400},
            {5, 1791043200},
            {3, 1806768000},
            {5, 1822492800},
            {3, 1838217600},
            {5, 1853942400},
            {3, 1869667200},
            {5, 1885996800},
            {3, 1901721600},
            {5, 1917446400},
            {3, 1933171200},
            {5, 1948896000},
            {3, 1964620800},
            {5, 1980345600},
            {3, 1996070400},
            {5, 2011795200},
            {3, 2027520000},
            {5, 2043244800},
            {3, 2058969600},
            {5, 2075299200},
            {3, 2091024000},
            {5, 2106748800},
            {3, 2122473600},
            {5, 2138198400},
        }};
    }

    inline constexpr db::entry macquarie{"Antarctica/Macquarie",
                                         "Macquarie Island",
                                         "-5430+15857",
                                         detail::macquarie::country_codes,
                                         detail::macquarie::offsets,
                                         detail::macquarie::transitions};

    // clang-format on
}
